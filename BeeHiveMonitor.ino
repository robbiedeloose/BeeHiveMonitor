/*

  genuino mkr1000n --> wifi101 library
  dht22 temp an humidity sensor

*/

/*
  Web client

  This sketch connects to a website (http://www.google.com)
  using a WiFi shield.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  Circuit:
   WiFi shield attached

  created 13 July 2010
  by dlf (Metodo2 srl)
  modified 31 May 2012
  by Tom Igoe
*/


#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "telenet-replica"; //  your network SSID (name)
char pass[] = "newyork20newyork15";    // your network password (use for WPA, or use as key for WEP)

// zee
//char ssid[] = "telenet-224D7"; //  your network SSID (name)
//char pass[] = "Zcmssfbu3dvj";    // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

/////////////////
// Phant Stuff //
/////////////////
const String publicKey = "1n6YvZb9QYsEDamb2pmM";
const String privateKey = "0mjXKloeVXCNxwgKVjgZ";
char server[] = "data.sparkfun.com"; // Remote host site
//const byte NUM_FIELDS = 3;
//const String fieldNames[NUM_FIELDS] = {"light", "switch", "name"};
//String fieldData[NUM_FIELDS];

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

//// DEFINE AREA SENSORS ////////////////////////////////////////////////////////////////////////////////////////////////////////////

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
// http://www.pjrc.com/teensy/td_libs_OneWire.html
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library

/* YourDuino TEST: Temperature Sensor DS18B20
    V1.2 08-26-2016
  - Connect cable to Arduino Digital I/O Pin 2
  terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <OneWire.h>
#include <DallasTemperature.h>

/*-----( Declare Constants )-----*/
#define ONE_WIRE_BUS 3 /*-(Connect to Pin 2 )-*/

/*-----( Declare objects )-----*/
/* Set up a oneWire instance to communicate with any OneWire device*/
OneWire ourWire(ONE_WIRE_BUS);

/* Tell Dallas Temperature Library to use oneWire Library */
DallasTemperature sensors(&ourWire);

/*-----( Declare Variables )-----*/

// on pin 3 (a 4.7K resistor is necessary)

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <DHT.h>
#define DHTPIN 2     // what digital pin we're connected to
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

//// END DEFINE AREA SENSORS ////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {

  //// SETUP AREA SENSORS ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  dht.begin();
  //// END SETUP AREA SENSORS ////////////////////////////////////////////////////////////////////////////////////////////////////////

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }

/*-( Start up the DallasTemperature library )-*/
sensors.begin();
  
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  postData();
  readSensors();
  delay(10000);
}

void postData()
{

  if (client.connect(server, 80))
  {
    Serial.println(F("Connected"));

    // Post the data! Request should look a little something like:
    // GET /input/publicKey?private_key=privateKey&light=1024&switch=0&time=5201 HTTP/1.1\n
    // Host: data.sparkfun.com\n
    // Connection: close\n
    // \n
    client.print("GET /input/");
    client.print(publicKey);
    client.print("?private_key=");
    client.print(privateKey);

    /*for (int i=0; i<NUM_FIELDS; i++)
      {
      client.print("&");
      client.print(fieldNames[i]);
      client.print("=");
      client.print(fieldData[i]);
      }*/

    client.print("&");
    client.print("humidity");
    client.print("=");
    client.print("85");

    client.print("&");
    client.print("temp_outside");
    client.print("=");
    client.print("12");

    client.print("&");
    client.print("temp_1");
    client.print("=");
    client.print("26");

    client.print("&");
    client.print("temp_2");
    client.print("=");
    client.print("35");

    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();

    // While we're connected, print out anything the server sends:
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.print(c);
      }
    }
    Serial.println();
  }
  else // If the connection failed, print a message:
  {
    Serial.println(F("Connection failed"));
  }

  // If we're disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println(F("Disconnecting."));
    client.stop();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void readSensors () {

  Serial.println();
Serial.print("Requesting temperature...");
sensors.requestTemperatures(); // Send the command to get temperatures
Serial.println("DONE");

Serial.print("Device 1 (index 0) = ");
Serial.print(sensors.getTempCByIndex(0));
Serial.println(" Degrees C");
Serial.print("Device 1 (index 0) = ");
Serial.print(sensors.getTempFByIndex(0));
Serial.println(" Degrees F");

Serial.print("Device 1 (index 1) = ");
Serial.print(sensors.getTempCByIndex(1));
Serial.println(" Degrees C");
Serial.print("Device 1 (index 1) = ");
Serial.print(sensors.getTempFByIndex(1));
Serial.println(" Degrees F");

  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

}


