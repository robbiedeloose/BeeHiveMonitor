void postDataToSparkFun()
{
  // Post Hive Data
  Serial.print("Posting Data for ");
  Serial.print(hives);
  Serial.println(" hives");

  for (int hive = 0; hive < hives; hive++) {

    if (client.connect(server, 80))
    {
      Serial.println(F("Connected"));

      // Post the data! Request should look a little something like:
      // GET /input/publicKey?private_key=privateKey&light=1024&switch=0&time=5201 HTTP/1.1\n
      // Host: data.sparkfun.com\n
      // Connection: close\n
      // \n
      client.print("GET /input/");
      client.print(publicKey[hive]);
      client.print("?private_key=");
      client.print(privateKey[hive]);

      /*for (int i=0; i<NUM_FIELDS; i++)
        {
        client.print("&");
        client.print(fieldNames[i]);
        client.print("=");
        client.print(fieldData[i]);
        }*/

      client.print("&");
      client.print("time_and_date");
      client.print("=");
      client.print("-");

      client.print("&");
      client.print("hive");
      client.print("=");
      client.print(hiveName[hive]);

      client.print("&");
      client.print("temperature_outside");
      client.print("=");
      client.print(weather_temp);

      //print relevant DS sensors for this hive
      for (int sensor = 0; sensor < 3; sensor++) {
        client.print("&");
        client.print("temperature_");
        int z = sensor + 1;
        client.print(z);
        client.print("=");
        client.print(hives_temp[hive][sensor]);
      }

      client.print("&");
      client.print("humidity_outside");
      client.print("=");
      client.print(weather_humidity);

      client.print("&");
      client.print("humidity_inside");
      client.print("=");
      client.print(hive_humidity[hive]);

      client.print("&");
      client.print("weight");
      client.print("=");
      client.print("-");

      client.print("&");
      client.print("battery");
      client.print("=");
      client.print(system_bat);

      client.print("&");
      client.print("battery_2");
      client.print("=");
      client.print(system_bat2);

      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.println(server);
      client.println("Connection: close");
      client.println();

      //  While we're connected, print out anything the server sends:
      while (client.connected())
      {
        if (client.available())
        {
          char c = client.read();
          Serial.print(c);
        }
      }
      Serial.println();
    }
    else // If the connection failed, print a message:
    {
      Serial.println(F("Connection failed"));
    }

    // If we're disconnected, stop the client:
    if (!client.connected())
    {
      Serial.println(F("Disconnecting."));
      client.stop();

    }
    delay(1000);
  }

  // Post Weather Data

  Serial.print("Posting Data Weather Station:");
  Serial.println(weatherStation);

  if (weatherStation)
  {
    if (client.connect(server, 80))
    {
      Serial.println(F("Connected"));

      client.print("GET /input/");
      client.print(publicWeatherKey);
      client.print("?private_key=");
      client.print(privateWeatherKey);

      client.print("&");
      client.print("temperature");
      client.print("=");
      client.print(weather_temp);

      client.print("&");
      client.print("humidity");
      client.print("=");
      client.print(weather_humidity);

      client.print("&");
      client.print("pressure");
      client.print("=");
      client.print(weather_pressure);

      client.print("&");
      client.print("windspeed");
      client.print("=");
      client.print(weather_speed);

      client.print("&");
      client.print("winddirection");
      client.print("=");
      client.print(weather_direction);

      client.print("&");
      client.print("lux");
      client.print("=");
      client.print("-");

      client.print("&");
      client.print("rainfall");
      client.print("=");
      client.print("-");

      client.print("&");
      client.print("battery");
      client.print("=");
      client.print(system_bat);

      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.println(server);
      client.println("Connection: close");
      client.println();

      // While we're connected, print out anything the server sends:
      while (client.connected())
      {
        if (client.available())
        {
          char c = client.read();
          Serial.print(c);
        }
      }
      Serial.println();
    }
    else // If the connection failed, print a message:
    {
      Serial.println(F("Connection failed"));
    }

    // If we're disconnected, stop the client:
    if (!client.connected())
    {
      Serial.println(F("Disconnecting."));
      client.stop();

    }
    delay(1000);
  }
}

