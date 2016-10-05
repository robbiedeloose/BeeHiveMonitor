void readBattery(){
   int sensorValue = analogRead(BATTERYVOLTAGE);
   //                     (sensorValue * (3.3/1023.0) - cor miltimeter) / (voltage divider)
   batteryVoltage = (sensorValue * (3.3 / 1023.0) - 0.09) / (10/14.7);
   
   if(debug){
    Serial.print("Battery voltage: ");
    Serial.println(batteryVoltage);
    Serial.println();
   }
}

