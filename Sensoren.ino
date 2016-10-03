void readSensors () {

  sensors.requestTemperatures(); // Send the command to get temperatures

  temp1 = sensors.getTempCByIndex(0);
  temp2 = sensors.getTempCByIndex(1);

  Serial.print("Device 1 (index 0) = ");
  Serial.print(temp1);
  Serial.println(" Degrees C");

  Serial.print("Device 1 (index 1) = ");
  Serial.print(temp2);
  Serial.println(" Degrees C");
  


  /*  DHT Sensor  */

  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temp = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  heatIndex = dht.computeHeatIndex(temp, humidity, false);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" *C ");
  Serial.print("Heat index: ");
  Serial.print(heatIndex);
  Serial.println(" *C ");
}

