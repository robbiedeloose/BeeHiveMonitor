void initiateDsSensors() {
  // Start up the library
  sensors.begin();
  delay(1000);
  numberOfDevices = sensors.getDeviceCount();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  delay(1000);
  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  delay(1000);
  
  // Loop through each device, print out address
  for (int i = 0; i < numberOfDevices; i++)
  {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i))
    {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
    delay(1000);
  }
}

void alternateReadDsSensors()
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus

  if (debug) {
    Serial.println();
    Serial.println("DEBUG: Alternate DS2 sensors:");
  }
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  delay(1000);
  Serial.println("DONE");

  hives_temp [0][0] = sensors.getTempC(hive_1_1);
  delay(250);
  hives_temp [0][1] = sensors.getTempC(hive_1_2);
  delay(250);
  hives_temp [0][2] = sensors.getTempC(hive_1_3);
  delay(250);

  Serial.println(hives_temp [0][0]);
  Serial.println(hives_temp [0][1]);
  Serial.println(hives_temp [0][2]);
}


// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.println(tempC);

}
