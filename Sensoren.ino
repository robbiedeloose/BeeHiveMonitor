void readSensors () {

  //DS2 Sensors
  sensors.requestTemperatures(); // Send the command to get temperatures
  temp1 = sensors.getTempCByIndex(0);
  temp2 = sensors.getTempCByIndex(1);

  if (debug) {
    Serial.println("Debug DS18LS20 Sensors:");
    Serial.print("Device 1 (index 0) = ");
    Serial.print(temp1);
    Serial.println(" Degrees C");

    Serial.print("Device 1 (index 1) = ");
    Serial.print(temp2);
    Serial.println(" Degrees C");
    Serial.println();
  }

  /*  DHT Sensor  */
  humidity = dht.readHumidity();
  temp = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  heatIndex = dht.computeHeatIndex(temp, humidity, false);

  if (debug) {
    Serial.println("Debug DHT22 Sensors:");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *C ");
    Serial.print("Heat index: ");
    Serial.print(heatIndex);
    Serial.println(" *C ");
    Serial.println();
  }
}

void readPressure() {
  char status;
  double T, P, p0, a;

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:

  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE, 0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE * 3.28084, 0);
  Serial.println(" feet");

  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T, 2);
      Serial.print(" deg C, ");
      Serial.print((9.0 / 5.0)*T + 32.0, 2);
      Serial.println(" deg F");

      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P, 2);
          Serial.print(" mb, ");
          Serial.print(P * 0.0295333727, 2);
          Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P, ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0, 2);
          Serial.print(" mb, ");
          Serial.print(p0 * 0.0295333727, 2);
          Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P, p0);
          Serial.print("computed altitude: ");
          Serial.print(a, 0);
          Serial.print(" meters, ");
          Serial.print(a * 3.28084, 0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  delay(5000);  // Pause for 5 seconds.
}

void readHumidity() {
  Serial.print("Humidity(%RH): ");
  Serial.print(SHT2x.GetHumidity());
  Serial.print("     Temperature(C): ");
  Serial.println(SHT2x.GetTemperature());
}
