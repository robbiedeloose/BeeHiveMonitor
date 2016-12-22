// READ BATTERY VOLTAGE

void readBattery() {
  int sensorValue = analogRead(BATTERYVOLTAGE);
  system_bat = (sensorValue * (3.3 / 1023.0)) * 2 ;

  if (debug) {
    Serial.println("DEBUG battery: ");
    Serial.print("Battery voltage: ");
    Serial.println(system_bat);
    Serial.println();
  }
}

