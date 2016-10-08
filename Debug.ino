
void setDebug() {
  if (digitalRead(DEBUGPIN) == LOW) {
    debug = true;
    Serial.println("Debug is enabled!");
    debugMessage(1, 500);
  }
  else {
    debug = false;
    Serial.println("Debug is disabled!");
    debugMessage(2, 500);
  }
}

void debugMessage(int x, int y) {
  if (debug) {
    i = 0;
    while (i < x)
    {
      i++;
      digitalWrite(LED, HIGH);    // Toggle LED
      delay(y);
      digitalWrite(LED, LOW);    // Toggle LED
      delay(200);
    }
  }
}


