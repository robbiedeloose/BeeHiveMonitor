void postDataToSparkFun()
{
  // Post Hive Data
  Serial.print("Posting Data for ");
  Serial.print(hives);
  Serial.println(" hives");

  for (int x = 0; x < hives; x++) {

    if (client.connect(server, 80))
    {
      Serial.println(F("Connected"));

      // Post the data! Request should look a little something like:
      // GET /input/publicKey?private_key=privateKey&light=1024&switch=0&time=5201 HTTP/1.1\n
      // Host: data.sparkfun.com\n
      // Connection: close\n
      // \n
      client.print("GET /input/");
      client.print(publicKey[x]);
      client.print("?private_key=");
      client.print(privateKey[x]);

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
      client.print(hiveName[x]);

      client.print("&");
      client.print("temperature_outside");
      client.print("=");
      client.print(weather_temp);

      // calculate sensor id
      int max = (x + 1) * sensorsPerHive;
      int min = max - (sensorsPerHive);
      int z = 1;
      Serial.print("x: ");
      Serial.println(x);
      Serial.print("min: ");
      Serial.println(min);
      Serial.print("max: ");
      Serial.println(max);

      
        
       if (x==0){
        //print Hive 1 data
           for (int index = 0;  < 3; y++) {
               client.print("&");
               client.print("temperature_");
               client.print(z);
               client.print("=");
               client.print(hive_1_temp[y]);
           }
       } else if (x==1){
          //print Hive 2 data 
            for (int index = 0;  < 3; y++) {
               client.print("&");
               client.print("temperature_");
               client.print(z);
               client.print("=");
               client.print(hive_2_temp[y]);
           }
       } else if (x==2){
          //print Hive 3 data
            for (int index = 0;  < 3; y++) {
               client.print("&");
               client.print("temperature_");
               client.print(z);
               client.print("=");
               client.print(hive_3_temp[y]);
           }
       }
   
      client.print("&");
      client.print("humidity_outside");
      client.print("=");
      client.print(weather_humidity);

      client.print("&");
      client.print("humidity_inside");
      client.print("=");
      client.print(hive_humidity[x]);

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

