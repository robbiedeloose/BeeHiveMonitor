#include <Wire.h> // I2C library
#include <RTCZero.h> // RealTimeClock for sleep
#include <SPI.h>
#include <WiFi101.h>
#include <DHT.h> // for DHT Sensors
#include <OneWire.h> // for DS18 sensors
#include <DallasTemperature.h>  // for DS18 sensors

#define ONE_WIRE_BUS 3 // Digital pin for OneWire sensor
#define DHTPIN 2     // what digital pin DHT is connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define LED 6 // Led Pin
#define DEBUGPIN 7 // pin for debug switch
#define BATTERYVOLTAGE A0 // pin for voltage divider

WiFiClient client; // network client
DHT dht(DHTPIN, DHTTYPE); // DHT instance
RTCZero rtc; // real time clock instance
OneWire ourWire(ONE_WIRE_BUS); // Set up a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&ourWire); // Tell Dallas Temperature Library to use oneWire Library

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
float windSnelheid = 0; // second arduino
int windRichting = 0; // second arduino
float humidity = 0; //DHT
float temp = 0; // DHT
float heatIndex = 0; //DHT
float temp1 = 0; //DS2-1
float temp2 = 0; //DS2-2
float batteryVoltage = 0;

void setup() {
  //Set pinmodes
  pinMode(LED, OUTPUT);
  pinMode(DEBUGPIN, INPUT_PULLUP);
  pinMode (BATTERYVOLTAGE, INPUT);

  Serial.begin(9600); // Start Serial
  Wire.begin();       // join i2c bus for wind sensor
  dht.begin();        // Start DHT22 sensor
  sensors.begin();    // Start up the DallasTemperature library
  initiateWifi();     // Start Wifi Connection
  setDebug();
  delay(5000);

  getTimeFromWeb(client);
  setRtc();
  WiFi.lowPowerMode();
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
    delay(2000);
    readSensors();
    readWind();
    readBattery();
    postDataToSparkFun();
    delay(1000);

    if (debug == false) {
      Serial.println("Debug mode OFF");
      sleep = true;
      rtc.standbyMode();
    }
    Serial.println("end process");
  }
  Serial.println("end loop");
}



