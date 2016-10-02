void readWind(){
  Wire.requestFrom(2, 4);    // request 4 bytes from slave device #2


  byte a, b, c, d;

  a = Wire.read();
  b = Wire.read();
  c = Wire.read();
  d = Wire.read();

  int WSH = 0;

  windRichting = a;
  windRichting = windRichting << 8 | b;
  Serial.print("Graden: ");
  Serial.print(windRichting);
  Serial.println("*");
  WSH = c;
  WSH = WSH << 8 | d;
  windSnelheid = WSH;
  windSnelheid = windSnelheid / 100;
  Serial.print("Snelheid: ");
  Serial.print(windSnelheid);
  Serial.println("Kn");

  //Serial.println(a);
  //Serial.println(b);
  //Serial.println(c);
  //Serial.println(d);
}

