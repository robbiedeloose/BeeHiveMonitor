void setRtc() {
  if (debug) {
    Serial.println("DEBUG RTC:");
    Serial.println("start RTC");
  }
  rtc.begin();
  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(days, months, years);
  if (debug) {
    Serial.println("set alarm");
  }
  rtc.setAlarmTime(0, 0, 0);
  rtc.enableAlarm(rtc.MATCH_SS);
  rtc.attachInterrupt(alarmMatch);
}

void alarmMatch()
{
  if (debug) {
    Serial.println("alarm triggered");
  }
  sleep = false;
}

void setNextReboot() {
  epochNextReboot = rtc.getEpoch() + 100;
  if (debug) {
    Serial.print("Current Epoch time: "); Serial.println(rtc.getEpoch());
    Serial.print("Next reboot: "); Serial.println(epochNextReboot);
  }
}

void checkIfRebootIsNeeded() {
  epochTime = rtc.getEpoch();
  if( epochTime > epochNextReboot){
    Serial.println("Ready to reboot");
    delay(2000);
    resetFunc(); //call reset 
  }
  else{
     Serial.println("Ready timer not yet reached");
  }
}
