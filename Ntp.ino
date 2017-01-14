
void getTimeFromWeb (Client &client)
{
  unsigned long time = 0;

  // Just choose any reasonably busy web server, the load is really low
  if (client.connect("g.cn", 80))
  {
    // Make an HTTP 1.1 request which is missing a Host: header
    // compliant servers are required to answer with an error that includes
    // a Date: header.
    client.print(F("GET / HTTP/1.1 \r\n\r\n"));

    char buf[5];      // temporary buffer for characters
    client.setTimeout(5000);
    if (client.find((char *)"\r\nDate: ") // look for Date: header
        && client.readBytes(buf, 5) == 5) // discard
    {
      day = client.parseInt();    // day
      client.readBytes(buf, 1);    // discard
      client.readBytes(buf, 3);    // month
      year = client.parseInt();    // year
      hours = client.parseInt();   // hour
      minutes = client.parseInt(); // minute
      seconds = client.parseInt(); // second

      switch (buf[0])
      {
        case 'F': month = 2; break; // Feb
        case 'S': month = 9; break; // Sep
        case 'O': month = 10; break; // Oct
        case 'N': month = 11; break; // Nov
        case 'D': month = 12; break; // Dec
        default:
          if (buf[0] == 'J' && buf[1] == 'a')
            month = 1;   // Jan
          else if (buf[0] == 'A' && buf[1] == 'p')
            month = 4;    // Apr
          else switch (buf[2])
            {
              case 'r': month = 3; break; // Mar
              case 'y': month = 5; break; // May
              case 'n': month = 6; break; // Jun
              case 'l': month = 7; break; // Jul
              default: // add a default label here to avoid compiler warning
              case 'g': month = 8; break; // Aug
            }
      }

    }
  }

  hours = hours + 2; // add 2 hours to get to right time zone
  if (hours >= 24) {
    hours = hours - 24; // adjust for 22 & 23
  }

  if (debug) {
    Serial.println("");
    Serial.println("Time:");
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(":");
    Serial.println(seconds);

    Serial.println("Date:");
    Serial.print(day);
    Serial.print("/");
    Serial.print(month);
    Serial.print("/");
    Serial.println(year);
  }
  delay(10);
  client.flush();
  client.stop();
}




