void postDataToSparkFun()
{

  if (client.connect(server, 80))
  {
    Serial.println(F("Connected"));

    // Post the data! Request should look a little something like:
    // GET /input/publicKey?private_key=privateKey&light=1024&switch=0&time=5201 HTTP/1.1\n
    // Host: data.sparkfun.com\n
    // Connection: close\n
    // \n
    client.print("GET /input/");
    client.print(publicKey);
    client.print("?private_key=");
    client.print(privateKey);

    /*for (int i=0; i<NUM_FIELDS; i++)
      {
      client.print("&");
      client.print(fieldNames[i]);
      client.print("=");
      client.print(fieldData[i]);
      }*/

    client.print("&");
    client.print("humidity");
    client.print("=");
    client.print("-");

    client.print("&");
    client.print("temp_outside");
    client.print("=");
    client.print(temp);

    client.print("&");
    client.print("temp_1");
    client.print("=");
    client.print(temp1);

    client.print("&");
    client.print("temp_2");
    client.print("=");
    client.print(temp2);

    client.print("&");
    client.print("humidity_outside");
    client.print("=");
    client.print(humidity);

    client.print("&");
    client.print("rainfall");
    client.print("=");
    client.print("-");

    client.print("&");
    client.print("winddirection");
    client.print("=");
    if (windMeter == 0) {
      client.print("-");
    }
    else {
      client.print(windRichting);
    }

    client.print("&");
    client.print("windspeed");
    client.print("=");
    if (windMeter == 0) {
      client.print("-");
    }
    else {
      client.print(windSnelheid);
    }

    client.print("&");
    client.print("battery");
    client.print("=");
    client.print(batteryVoltage);

    client.print("&");
    client.print("weight");
    client.print("=");
    if (scale == 0) {
      client.print("-");
    }
    else {
      client.print("-");
    }


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
}
