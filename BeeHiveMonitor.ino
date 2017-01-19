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
#define DEBUGPIN A0 // pin for debug switchx  
#define BATTERYVOLTAGE A7 // pin for voltage divider
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

String publicKey[] = {"azerty", "RM7wrJGMD5uz9Yypm88z"};
String privateKey[] = {"qwerty", "lza1AWPz28hMz580AwwM"};
char server[] = "data.sparkfun.com"; // Remote host site

const String publicWeatherKey = "wpjxZLXmDwTrprnEwl1o";
const String privateWeatherKey = "wzEPBlDjyZUzqzgD9RWl";

// Config
int hives = 2;
String hiveName[] = {"Hive1", "Hive2"};
int sensorsPerHive = 3;
boolean scale = false;
boolean weatherStation = true;
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
const char CONFIG_FILE[] = "example.cfg";

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
char *hello;
boolean doDelay;
int waitMs;

boolean readConfiguration();

/////////////////////////////////////////////////////////////////////////////////

void setup() {

  delay(5000);

  //Set pinmodes
  pinMode(LED, OUTPUT);
  pinMode(DEBUGPIN, INPUT_PULLUP);
  pinMode (BATTERYVOLTAGE, INPUT);
  pinMode (BATTERYVOLTAGE2, INPUT);

  Serial.begin(9600); // Start Serial
  delay(2000);
  //read sd first, the choose the correct startup actions
  pinMode(pinSelectSD, OUTPUT);
  didReadConfig = false;
  hello = 0;
  doDelay = false;
  waitMs = 0;

  Serial.println();
  Serial.println("----- Starting SD Card");
  setupSdCard();
  testsd();
  Serial.println();
  Serial.println("----- Reading configuration data from SD card");
  //didReadConfig = readConfiguration();

  Serial.println();
  Serial.println("----- Start I2C Bus");
  Wire.begin();       // join i2c bus for wind sensor
  Serial.println();
  Serial.println("----- Start Dallas Library");
  sensors.begin();    // Start up the DallasTemperature library
  Serial.println();
  Serial.println("----- Start Wifi Connection");
  initiateWifi();     // Start Wifi Connection
  Serial.println();
  Serial.println("----- Set Debug Level");
  setDebug();         // Check if debug is enabled (pin 10 low)
  delay(10000);        // Allow some time before sleep to be able to reprogram te board if needed
  Serial.println();
  Serial.println("----- Get Time From the Web");
  getTimeFromWeb(client);
  Serial.println();
  Serial.println("----- Set Real Time Clock");
  setRtc();
  Serial.println("set wifi power mode");
  WiFi.lowPowerMode();
  Serial.println("start bmp180");

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin()) {
    delay(1000);
    Serial.println("BMP180 init success");
  }
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    //while(1); // Pause forever.
  }
}

void loop() {

  Serial.println();
  Serial.println();
  Serial.println("Going Through Loop");
  Serial.println();
  Serial.println();

  debugMessage(1, 200); // going throug loop
  /*  Check incomming HTTP:  */
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  if (sleep == false) {

    if (loopCount == interval) {
      debugMessage(5, 100);

      // READ HIVE TEMPERATURES
      readDS2Sensors();

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


      testsd();
      logToSdCard();


      postDataToSparkFun();
      delay(1000);



      if (debug == false) {
        if (debug) {
          Serial.println("Sleeping");
        }
        sleep = true;
        rtc.standbyMode();
      }
      loopCount = 0;
    }
    loopCount ++;
  }
}



