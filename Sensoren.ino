// READ WEATHER DATA --> TEMP AND HUMIDITY

void readHTU21D() {
  weather_temp = SHT2x.GetTemperature();
  weather_humidity = SHT2x.GetHumidity();
  if (debug) {
    Serial.println();
    Serial.println("DEBUG HTU21 Sensor:");
    Serial.print  ("Humidity(%RH): ");
    Serial.println(weather_temp);
    Serial.print("Temperature(C): ");
    Serial.println(weather_humidity);
  }
}

// READ HIVE HUMIDITY

void readDhtSensors () {
  hive_humidity[0] = dht1.readHumidity();
  delay(500);

  if (debug) {
    Serial.println("Debug DHT22 Sensors:");
    Serial.print("Humidity: ");
    Serial.print(hive_humidity[0]);
    Serial.println("%");
  }
}
