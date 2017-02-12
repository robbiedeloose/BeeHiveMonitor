  #include <Wire.h> // I2C library
#include <RTCZero.h> // RealTimeClock for sleep 
#include <RTClib.h>
#include <SPI.h>
#include <WiFi101.h>
#include <SD.h>
#include <SDConfigFile.h>
#include <OneWire.h> // for DS18 sensors
#include <DallasTemperature.h>  // for DS18 sensors
#include <SHT2x.h> // weather humidity
#include <DHT.h> // hive humidity

///////////////// !!!!! PINS !!!!! ///////////////////////////////////////////////
#define ONE_WIRE_BUS A5 // Digital pin for OneWire sensor                       //
// LED13 on feather LED 6 on MKR                                                //
#define LED 13 // Led Pin                                                       //
#define DEBUGPIN A0 // pin for debug switch                                     //
#define BATTERYVOLTAGE A7// pin for voltage divider                             //
#define BATTERYVOLTAGE2 A1 // pin for 12v                                       //
#define ALTITUDE 0.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters     //
#define DHTPIN1 A2 // what digital pin we're connected to                       //
//#define DHTPIN2 A3 // what digital pin we're connected to                     //
//#define DHTPIN3 A4 // what digital pin we're connected to                     //
#define SCALE1_CLK 11                                                           //
#define SCALE1_DTA 12                                                           //
//#define SCALE2_CLK 5                                                          //
//#define SCALE2_DTA 6                                                          //
//#define SCALE3_CLK 0                                                          //
//#define SCALE3_DTA 1                                                          //
//////////////////////////////////////////////////////////////////////////////////

#define ALTITUDE 0.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

WiFiClient client; // network client
RTCZero rtc; // real time clock instance
OneWire ourWire(ONE_WIRE_BUS); // Set up a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&ourWire); // Tell Dallas Temperature Library to use oneWire Library

DHT dht1(DHTPIN1, DHTTYPE);
//DHT dht2(DHTPIN2, DHTTYPE);
//DHT dht3(DHTPIN3, DHTTYPE);

////// Config variables //////

// Wifi
boolean useWifi = true;
//char *ssid = "dd-wrt"; //  your network SSID (name)
char *ssid = "dd-wrt"; //  your network SSID (name)
char *pass = "newyork20newyork15";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
boolean useGprs = false;

// Phant
//char *publicKey1;
//char *privateKey1;

String publicKey[] = {"RM7wrJGMD5uz9Yypm88z", "RM7wrJGMD5uz9Yypm88z"};
String privateKey[] = {"lza1AWPz28hMz580AwwM", "lza1AWPz28hMz580AwwM"};
char server[] = "data.sparkfun.com"; // Remote host site

const String publicWeatherKey = "wpjxZLXmDwTrprnEwl1o";
const String privateWeatherKey = "wzEPBlDjyZUzqzgD9RWl";

DeviceAddress hive_1_1 = { 0x28, 0xFF, 0x4C, 0xB8, 0x92, 0x16, 0x04, 0x4D };
DeviceAddress hive_1_2 = { 0x28, 0xFF, 0x93, 0xEE, 0x89, 0x16, 0x03, 0xA5 };
DeviceAddress hive_1_3 = { 0x28, 0xFF, 0x3F, 0x0B, 0x8A, 0x16, 0x03, 0x9B };
DeviceAddress hive_2_1;
DeviceAddress hive_2_2;
DeviceAddress hive_2_3;
DeviceAddress hive_3_1;
DeviceAddress hive_3_2;
DeviceAddress hive_3_3;

// Config
int hives = 1;
String hiveName[] = {"Hive1", "Hive2"};
int sensorsPerHive = 3;
boolean scale = false;
boolean weatherStation = false;
int interval = 5; // in minuten

const String boardType = "feather"; // feather

////// Variables //////

// WiFi status
int status = WL_IDLE_STATUS;
// Date and Time
byte seconds = 0;
byte minutes = 00;
byte hours = 17;
byte days = 17;
byte months = 11;
int years = 15;
time_t epochTime;
time_t epochNextReboot;

// sleep & debug
boolean sleep = false;
boolean debug = true;
int i = 0;

// Sensors

int numberOfDevices; // Number of temperature devices found
DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

float weather_speed = 0; // second arduino
int weather_direction = 0; // second arduino
int weather_lux = 0; // not there yet
float weather_rainfall = 0; // not there yet
float weather_humidity = 0; //HTU21D
float weather_temp = 0; // HTU21D
float hive_temp[15]; //DS2
float hive_1_temp [3]; // ds 2 sensors for hive 1
float hive_2_temp [3]; // ds 2 sensors for hive 2
float hive_3_temp [3]; // ds 2 sensors for hive 3
float hives_temp [3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
float hive_humidity[3] = {0, 0, 0}; //DHT22
float system_bat = 0;
float system_bat2 = 0;

// SD CARD //////////////////////////////////////////////////////////////////////

const int pinSelectSD = 10; // SD shield Chip Select pin.

// The filename of the configuration file on the SD card
// dualHive.cfg or singleHive.cfg
const char CONFIG_FILE[] = "singleHive.cfg";

/*
   Settings we read from the configuration file.
     didReadConfig = true if the configuration-reading succeeded;
       false otherwise.
       Used to prevent odd behaviors if the configuration file
       is corrupt or missing.
     hello = the "hello world" string, allocated via malloc().
     doDelay = if true, delay waitMs in loop().
       if false, don't delay.
     waitMs = time (milliseconds) to wait after printing hello.
*/
boolean didReadConfig = false;
boolean readConfiguration();

String httpData;
String sdData;

/////////////////////////////////////////////////////////////////////////////////

void setup() {

  Serial.begin(9600);
  delay(2000);
  Serial.println("Reprogram windows: 10 seconds");
  delay(100000);

  setPinmodes();

  Serial.println("-- Starting SD Card");
  setupSdCard(); // start sd card
  Serial.println("-- Reading configuration data from SD card");
  didReadConfig = readConfiguration();

  if (didReadConfig) {
    applicationLog("-- Read settings from SD config file");
  } else {
    applicationLog("-- Unable to read settings file from SD card");
  }


  Wire.begin();       // join i2c bus for wind sensor and other I2C sensors
  sensors.begin();    // Start up the DallasTemperature library
  initiateDsSensors();

  Serial.println("--Start Wifi Connection");
  initiateWifi();     // Start Wifi Connection
  Serial.println();

  Serial.println("----- Get Time From the Web");
  getTimeFromWeb(client);

  Serial.println("----- Set Real Time Clock");
  setRtc();
  setNextReboot();

  Serial.println("set wifi power mode");
  WiFi.lowPowerMode();

  logRebootInfoToSdCard();
}

void(* resetFunc) (void) = 0;//declare reset function at address 0

void loop() {

  if (sleep == false) {
    Serial.println("Going Through Loop");
    debugMessage(1, 200); // going throug loop
    applicationLog("loop");
    doStuf();
    checkIfRebootIsNeeded();
    // Delay for 60 seconds if debug is enabled, sleep otherwise
    if (debug == true) {
      delay(60000);
    }
    else {
      sleep = true;
      rtc.standbyMode();
    }
  }
}

void doStuf() {

  debugMessage(5, 100);
  alternateReadDsSensors(); // READ HIVE TEMPERATURES
  readDhtSensors(); // READ HIVE HUMIDITY
  readHTU21D(); // READ WEATHER DATA --> TEMP AND HUMIDITY

  readBattery(); // READ BATTERY VOLTAGE
  readBattery2(); // READ BATTERY VOLTAGE

  logToSdCard(); // log to SD card
  postDataToSparkFun(); // send to the internet

  delay(1000);

}
