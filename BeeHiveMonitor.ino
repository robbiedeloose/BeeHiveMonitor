#include <Wire.h> // I2C library
// RTC - MKR1000
#include <RTCZero.h> // RealTimeClock for sleep 
// RTC - Adafruit Feather M0 WiFi
//#include <RTClib.h>
#include <SPI.h>
// Wifi - same for MKR1000 and Adafruit Feather M0 WiFi
#include <WiFi101.h>


#include <OneWire.h> // for DS18 sensors
#include <DallasTemperature.h>  // for DS18 sensors
#include <SFE_BMP180.h> // pressure
#include <SHT2x.h> // humidity

#define ONE_WIRE_BUS 5 // Digital pin for OneWire sensor
// LED13 on feather LED 6 on MKR
#define LED 13 // Led Pin
#define DEBUGPIN 10 // pin for debug switch
#define BATTERYVOLTAGE A7 // pin for voltage divider
#define ALTITUDE 0.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters


WiFiClient client; // network client
RTCZero rtc; // real time clock instance
OneWire ourWire(ONE_WIRE_BUS); // Set up a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&ourWire); // Tell Dallas Temperature Library to use oneWire Library
SFE_BMP180 pressure;



////// Config variables //////

// Wifi
char ssid[] = "telenet-replica"; //  your network SSID (name)
char pass[] = "newyork20newyork15";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

// Phant
const String publicKey = "1n6YvZb9QYsEDamb2pmM";
const String privateKey = "0mjXKloeVXCNxwgKVjgZ";
char server[] = "data.sparkfun.com"; // Remote host site

// Sensors
byte hives = 2;
byte sensorsPerHive = 2;
byte windMeter = 0;
byte scale = 0;

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
int   weather_direction = 0; // second arduino
float weather_humidity = 0; //DHT
float weather_temp = 0; // DHT
float hive_temp[15]; //DS2
float system_bat = 0;

void setup() {

  //read sd first, the choose the correct startup actions

  //Set pinmodes
  pinMode(LED, OUTPUT);
  pinMode(DEBUGPIN, INPUT_PULLUP);
  pinMode (BATTERYVOLTAGE, INPUT);

  Serial.begin(9600); // Start Serial
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
  debugMessage(1, 200); // going throug loop
  /*  Check incomming HTTP:  */
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  if (sleep == false) {
    debugMessage(5, 100);
    
    //readBmp180();
    readHTU21D();
    //readWind();
    readBattery();
    readDS2Sensors();
    
    postDataToSparkFun();
    delay(1000);

    if (debug == false) {
      if (debug){
        Serial.println("Sleeping");
      }
      sleep = true;
      rtc.standbyMode();
    }
  }
}



