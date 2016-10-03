void readWind() {
  Wire.requestFrom(2, 4);    // request 4 bytes from slave device #2

  byte a, b, c, d;
  a = Wire.read();
  b = Wire.read();
  c = Wire.read();
  d = Wire.read();

  windRichting = a;
  windRichting = windRichting << 8 | b;

  int WSH = 0;
  WSH = c;
  WSH = WSH << 8 | d;
  windSnelheid = WSH;
  windSnelheid = windSnelheid / 100;

  if (debug) {
    Serial.println("Debug wind Sensor:");
    Serial.print("Graden: ");
    Serial.print(windRichting);
    Serial.println("*");
    Serial.print("Snelheid: ");
    Serial.print(windSnelheid);
    Serial.println("Kn");
    Serial.println();
  }

  //Serial.println(a);
  //Serial.println(b);
  //Serial.println(c);
  //Serial.println(d);
}

