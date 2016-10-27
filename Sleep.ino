void setRtc() {
  if (debug) {
    Serial.println("DEBUG RTC:");
    Serial.println("start RTC");
  }
  rtc.begin();
  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);
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
