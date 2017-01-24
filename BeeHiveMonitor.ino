#include <Wire.h> // I2C library
// RTC - MKR1000
#include <RTCZero.h> // RealTimeClock for sleep 
// RTC - Adafruit Feather M0 WiFi
//#include <RTClib.h>
#include <SPI.h>
// Wifi - same for MKR1000 and Adafruit Feather M0 WiFi
#include <WiFi101.h>

#include <SD.h>
#include <SDConfigFile.h>

#include <OneWire.h> // for DS18 sensors
#include <DallasTemperature.h>  // for DS18 sensors
#include <SFE_BMP180.h> // pressure
#include <SHT2x.h> // weather humidity
#include <DHT.h> // hive humidity

///////////////// !!!!! PINS !!!!! /////////////////////////
#define ONE_WIRE_BUS A5 // Digital pin for OneWire sensor
// LED13 on feather LED 6 on MKR
#define LED 13 // Led Pin
#define DEBUGPIN A0 // pin for debug switch  
#define BATTERYVOLTAGE A7// pin for voltage divider
#define BATTERYVOLTAGE2 A1 // pin for 12v 
#define ALTITUDE 0.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
#define DHTPIN1 A2 // what digital pin we're connected to
//#define DHTPIN2 A3 // what digital pin we're connected to
//#define DHTPIN3 A4 // what digital pin we're connected to
#define SCALE1_CLK 11
#define SCALE1_DTA 12
//#define SCALE2_CLK 5
//#define SCALE2_DTA 6
//#define SCALE3_CLK 0
//#define SCALE3_DTA 1

#define ALTITUDE 0.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

WiFiClient client; // network client
RTCZero rtc; // real time clock instance
OneWire ourWire(ONE_WIRE_BUS); // Set up a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&ourWire); // Tell Dallas Temperature Library to use oneWire Library
SFE_BMP180 pressure;

DHT dht1(DHTPIN1, DHTTYPE);
//DHT dht2(DHTPIN2, DHTTYPE);
//DHT dht3(DHTPIN3, DHTTYPE);


////// Config variables //////

// Wifi
boolean useWifi = true;
char *ssid = "telenet-replica"; //  your network SSID (name)
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

// Config
int hives = 1;
String hiveName[] = {"Hive1", "Hive2"};
int sensorsPerHive = 3;
boolean scale = false;
boolean weatherStation = false;
int interval = 5; // in minuten
int loopCount = 0;

const String boardType = "feather"; // feather

////// Variables //////

// WiFi status
int status = WL_IDLE_STATUS;
// Date and Time
byte seconds = 0;
byte minutes = 00;
byte hours = 17;
byte day = 17;
byte month = 11;
int year = 15;

// sleep & debug
boolean sleep = false;
boolean debug = false;
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
double weather_pressure = 0;
float hive_temp[15]; //DS2
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
boolean didReadConfig;
boolean readConfiguration();

/////////////////////////////////////////////////////////////////////////////////

void setup() {

  delay(5000);

  //Set pinmodes
  pinMode(LED, OUTPUT);
  pinMode(DEBUGPIN, INPUT);
  pinMode (BATTERYVOLTAGE, INPUT);
  pinMode (BATTERYVOLTAGE2, INPUT);

  // Start Serial
  Serial.begin(9600);
  delay(2000);

  //read sd first, the choose the correct startup actions
  pinMode(pinSelectSD, OUTPUT);
  didReadConfig = false;

  Serial.println();
  Serial.println("-- Starting SD Card");
  setupSdCard();

  Serial.println("-- Reading configuration data from SD card");
  didReadConfig = readConfiguration();
  Serial.println("Done!");

  Wire.begin();       // join i2c bus for wind sensor and other I2C sensors
  sensors.begin();    // Start up the DallasTemperature library
  initiateDsSensors();

  Serial.println("--Start Wifi Connection");
  initiateWifi();     // Start Wifi Connection
  Serial.println();

  Serial.println("-- Set Debug Level");
  //setDebug();         // Check if debug is enabled (pin 10 low)
  debug = true;
  delay(10000);        // Allow some time before sleep to be able to reprogram te board if needed

  Serial.println("----- Get Time From the Web");
  getTimeFromWeb(client);

  Serial.println("----- Set Real Time Clock");
  setRtc();

  Serial.println("set wifi power mode");
  WiFi.lowPowerMode();

  //  Serial.println("start bmp180");
  //
  //  // Initialize the sensor (it is important to get calibration values stored on the device).
  //
  //  if (pressure.begin()) {
  //    delay(1000);
  //    Serial.println("BMP180 init success");
  //  }
  //  else
  //  {
  //    // Oops, something went wrong, this is usually a connection problem,
  //    // see the comments at the top of this sketch for the proper connections.
  //
  //    Serial.println("BMP180 init fail\n\n");
  //    //while(1); // Pause forever.
  //  }

}

void loop() {

  Serial.println("Going Through Loop");
  debugMessage(1, 200); // going throug loop

  //  /*  Check incomming HTTP:  */
  //  while (client.available()) {
  //    char c = client.read();
  //    Serial.write(c);
  //  }
  //
  //  if (sleep == false) {
  //    if (loopCount == interval) {
  //      Serial.println("Do Stuff");
  //
  //      if (debug == false)
  //      {
  //        if (debug) {
  //          Serial.println("Sleeping");
  //        }


    doStuf();

    // Delay for 60 seconds if debug is enabled, sleep otherwise
    if (debug == true){
      delay(60000);
    }
    else{
    rtc.standbyMode();  
    }


  //      }
  //      loopCount = 0;
  //    }
  //    loopCount ++;
  //  }
}

void doStuf() {
  debugMessage(5, 100);

  // READ HIVE TEMPERATURES
  //readDS2Sensors();
  readDsSensors();

  // READ HIVE HUMIDITY
  readDhtSensors();

  // READ WEATHER DATA --> TEMP AND HUMIDITY
  readHTU21D();

  // READ PRESSURE --> NOT USED ATM
  //readBmp180();

  // READ WIND DATA --> NOT USED ATM
  //readWind();

  // READ BATTERY VOLTAGE
  readBattery();
  readBattery2();

  // log to SD card
  logToSdCard();

  // send to the internet
  postDataToSparkFun();
  delay(1000);
}


