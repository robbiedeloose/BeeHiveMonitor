void readSensors () {

  //DS2 Sensors
  sensors.requestTemperatures(); // Send the command to get temperatures
  temp1 = sensors.getTempCByIndex(0);
  temp2 = sensors.getTempCByIndex(1);

  if (debug) {
    Serial.println("Debug DS18LS20 Sensors:");
    Serial.print("Device 1 (index 0) = ");
    Serial.print(temp1);
    Serial.println(" Degrees C");

    Serial.print("Device 1 (index 1) = ");
    Serial.print(temp2);
    Serial.println(" Degrees C");
    Serial.println();
  }

  /*  DHT Sensor  */
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  heatIndex = dht.computeHeatIndex(temp, humidity, false);
  
  if (debug) {
    Serial.println("Debug DHT22 Sensors:");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *C ");
    Serial.print("Heat index: ");
    Serial.print(heatIndex);
    Serial.println(" *C ");
    Serial.println();
  }
}
