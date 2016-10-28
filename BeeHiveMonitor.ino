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
#include <SHT2x.h> // humidity

#define ONE_WIRE_BUS 5 // Digital pin for OneWire sensor
// LED13 on feather LED 6 on MKR
#define LED 13 // Led Pin
#define DEBUGPIN 11 // pin for debug switch
#define BATTERYVOLTAGE A7 // pin for voltage divider
#define ALTITUDE 0.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters


WiFiClient client; // network client
RTCZero rtc; // real time clock instance
OneWire ourWire(ONE_WIRE_BUS); // Set up a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&ourWire); // Tell Dallas Temperature Library to use oneWire Library
SFE_BMP180 pressure;



////// Config variables //////

// Wifi
char *ssid = "dd-wrt"; //  your network SSID (name)
char pass[] = "newyork20newyork15";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

// Phant
const String publicKey[] = {"RM7wrJGMD5uz9Yypm88z", "RM7wrJGMD5uz9Yypm88z"};
const String privateKey[] = {"lza1AWPz28hMz580AwwM", "lza1AWPz28hMz580AwwM"};
char server[] = "data.sparkfun.com"; // Remote host site

const String publicWeatherKey = "wpjxZLXmDwTrprnEwl1o";
const String privateWeatherKey = "wzEPBlDjyZUzqzgD9RWl";

// Config
int hives = 1;
const String hiveName[] = {"Hive1", "Hive2"};
int sensorsPerHive = 3;
boolean scale = false;
boolean weatherStation = true;
boolean windMeter = false;
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
float weather_humidity = 0; //DHT
float weather_temp = 0; // DHT
double weather_pressure = 0;
float hive_temp[15]; //DS2
float system_bat = 0;

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

  //Set pinmodes
  pinMode(LED, OUTPUT);
  pinMode(DEBUGPIN, INPUT_PULLUP);
  pinMode (BATTERYVOLTAGE, INPUT);

  Serial.begin(9600); // Start Serial
delay(2000);
  //read sd first, the choose the correct startup actions
  pinMode(pinSelectSD, OUTPUT);
  didReadConfig = false;
  hello = 0;
  doDelay = false;
  waitMs = 0;
  setupSdCard();
  readConfiguration();

  Wire.begin();       // join i2c bus for wind sensor
  sensors.begin();    // Start up the DallasTemperature library
  initiateWifi();     // Start Wifi Connection
  setDebug();         // Check if debug is enabled (pin 10 low)
  delay(10000);        // Allow some time before sleep to be able to reprogram te board if needed

  getTimeFromWeb(client);
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
  Serial.println("Loop");
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

      readBmp180();
      readHTU21D();
      readWind();
      readBattery();
      readDS2Sensors();

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



