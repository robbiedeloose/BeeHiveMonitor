/* Includes and variables for Wifi  */
#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "telenet-replica"; //  your network SSID (name)
char pass[] = "newyork20newyork15";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiClient client;

/* Includes and variables for Phant Stuff */
const String publicKey = "1n6YvZb9QYsEDamb2pmM";
const String privateKey = "0mjXKloeVXCNxwgKVjgZ";
char server[] = "data.sparkfun.com"; // Remote host site

/*  Includes and variables for DS temp sensor  */
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 3 // Digital pin for OneWire sensor

OneWire ourWire(ONE_WIRE_BUS); // Set up a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&ourWire); // Tell Dallas Temperature Library to use oneWire Library

/*  Includes and variables for DHT temp & humidity sensor  */
#include <DHT.h>
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

/*  Includes and variables for   */
#include <Wire.h>
float windSnelheid = 0;
int windRichting = 0;

/*  Variables for storing sensor data  */
/*DHT*/
float humidity = 0;
float temp = 0;
float heatIndex = 0;
/*DS20*/
float temp1 = 0;
float temp2 = 0;

boolean debug = true;
#define LED 6
#define DEBUGPIN 7
int i = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(DEBUGPIN,INPUT_PULLUP);

  setDebug();
  debugMessage(2);

  Serial.begin(9600); /*  Start Serial  */
  delay(1000);
  //  while (!Serial) {
  //    ;
  //  }

  Wire.begin();        // join i2c bus for wind sensor

  dht.begin(); /*  Start DHT22 sensor  */
  sensors.begin(); /*  Start up the DallasTemperature library  */
  initiateWifi(); /*  Start Wifi Connection  */

}

void loop() {

  debugMessage(3);

  /*  Check incomming HTTP:  */
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  /*  Read sensor data from DHT22 and MD20  */
  readSensors();
  readWind();

  /*  write sensor data to serial or sd card  */


  /*  Post data to web service   */
  postDataToSparkFun()  ;

  /* Wait for 10 seconds. Needs to be repalced by a sleep mechanism  */
  delay(10000);
}

void debugMessage(int x) {
  if (debug) {
    i = 0;
    while (i < x)
    {
      i++;
      digitalWrite(LED, HIGH);    // Toggle LED
      delay(100);
      digitalWrite(LED, LOW);    // Toggle LED
      delay(200);
    }
  }
}


void setDebug(){
  if (digitalRead(DEBUGPIN) == LOW){
    debug = true;
  }
  else{
    debug = false;
  }
}

