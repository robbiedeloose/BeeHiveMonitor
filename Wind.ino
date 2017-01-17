// READ WIND DATA --> NOT USED ATM

void readWind() {
  Wire.requestFrom(2, 4);    // request 4 bytes from slave device #2

  byte a, b, c, d;
  a = Wire.read();
  b = Wire.read();
  c = Wire.read();
  d = Wire.read();

  weather_direction = a;
  weather_direction = weather_direction << 8 | b;

  int WSH = 0;
  WSH = c;
  WSH = WSH << 8 | d;
  weather_speed = WSH;
  weather_speed = weather_speed / 100;

  if (debug) {
    Serial.println("Debug wind Sensor:");
    Serial.print("Graden: ");
    Serial.print(weather_direction);
    Serial.println("*");
    Serial.print("Snelheid: ");
    Serial.print(weather_speed);
    Serial.println("Kn");
    Serial.println();
  }
}

