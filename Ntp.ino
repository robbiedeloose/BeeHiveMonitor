
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
if (hours >= 24){
  hours = hours - 24; // adjust for 22 & 23 
}

Serial.println("Time:");
Serial.print(hours);
Serial.print(":");
Serial.print(minutes);
Serial.print(":");
Serial.println(hours);

Serial.println("Date:");
Serial.print(day);
Serial.print("/");
Serial.print(month);
Serial.print("/");
Serial.println(year);


Serial.println("");
  
  delay(10);
  client.flush();
  client.stop();
}













/*
   © Francesco Potortì 2013 - GPLv3

   Send an HTTP packet and wait for the response, return the Unix time
*/
/*
  unsigned long webUnixTime (Client &client)
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
      hour = client.parseInt();   // hour
      minute = client.parseInt(); // minute
      second = client.parseInt(); // second

      int daysInPrevMonths;
      switch (buf[0])
      {
        case 'F': daysInPrevMonths =  31; break; // Feb
        case 'S': daysInPrevMonths = 243; break; // Sep
        case 'O': daysInPrevMonths = 273; break; // Oct
        case 'N': daysInPrevMonths = 304; break; // Nov
        case 'D': daysInPrevMonths = 334; break; // Dec
        default:
          if (buf[0] == 'J' && buf[1] == 'a')
            daysInPrevMonths = 0;   // Jan
          else if (buf[0] == 'A' && buf[1] == 'p')
            daysInPrevMonths = 90;    // Apr
          else switch (buf[2])
            {
              case 'r': daysInPrevMonths =  59; break; // Mar
              case 'y': daysInPrevMonths = 120; break; // May
              case 'n': daysInPrevMonths = 151; break; // Jun
              case 'l': daysInPrevMonths = 181; break; // Jul
              default: // add a default label here to avoid compiler warning
              case 'g': daysInPrevMonths = 212; break; // Aug
            }
      }

      // This code will not work after February 2100
      // because it does not account for 2100 not being a leap year and because
      // we use the day variable as accumulator, which would overflow in 2149
      day += (year - 1970) * 365; // days from 1970 to the whole past year
      day += (year - 1969) >> 2;  // plus one day per leap year
      day += daysInPrevMonths;  // plus days for previous months this year
      if (daysInPrevMonths >= 59  // if we are past February
          && ((year & 3) == 0)) // and this is a leap year
        day += 1;     // add one day
      // Remove today, add hours, minutes and seconds this month
      time = (((day - 1ul) * 24 + hour) * 60 + minute) * 60 + second;
    }
  }
  delay(10);
  client.flush();
  client.stop();

  return time;
  }

*/

void printRealTime (long epoch) {

  //  long secsSince1900 = x;
  //
  //  Serial.print("Seconds since Jan 1 1900 = " );
  //  Serial.println(secsSince1900);
  //
  //  // now convert NTP time into everyday time:
  //  Serial.print("Unix time = ");
  //  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  //  const unsigned long seventyYears = 2208988800UL;
  //  // subtract seventy years:
  //  unsigned long epoch = secsSince1900 - seventyYears;
  // print Unix time:
  Serial.println(epoch);


  // print the hour, minute and second:
  Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
  Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
  Serial.print(':');
  if ( ((epoch % 3600) / 60) < 10 ) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
  Serial.print(':');
  if ( (epoch % 60) < 10 ) {
    // In the first 10 seconds of each minute, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.println(epoch % 60); // print the second
}



//void checkTime()
//{
//  sendNTPpacket(timeServer); // send an NTP packet to a time server
//  // wait to see if a reply is available
//  delay(1000);
//  if ( Udp.parsePacket() ) {
//    Serial.println("packet received");
//    // We've received a packet, read the data from it
//    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
//
//    //the timestamp starts at byte 40 of the received packet and is four bytes,
//    // or two words, long. First, esxtract the two words:
//
//    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
//    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
//    // combine the four bytes (two words) into a long integer
//    // this is NTP time (seconds since Jan 1 1900):
//    unsigned long secsSince1900 = highWord << 16 | lowWord;
//    Serial.print("Seconds since Jan 1 1900 = " );
//    Serial.println(secsSince1900);
//
//    // now convert NTP time into everyday time:
//    Serial.print("Unix time = ");
//    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
//    const unsigned long seventyYears = 2208988800UL;
//    // subtract seventy years:
//    unsigned long epoch = secsSince1900 - seventyYears;
//    // print Unix time:
//    Serial.println(epoch);
//
//
//    // print the hour, minute and second:
//    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
//    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
//    Serial.print(':');
//    if ( ((epoch % 3600) / 60) < 10 ) {
//      // In the first 10 minutes of each hour, we'll want a leading '0'
//      Serial.print('0');
//    }
//    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
//    Serial.print(':');
//    if ( (epoch % 60) < 10 ) {
//      // In the first 10 seconds of each minute, we'll want a leading '0'
//      Serial.print('0');
//    }
//    Serial.println(epoch % 60); // print the second
//  }
//  // wait ten seconds before asking for the time again
//  delay(10000);
//}
//
//// send an NTP request to the time server at the given address
//unsigned long sendNTPpacket(IPAddress& address)
//{
//  //Serial.println("1");
//  // set all bytes in the buffer to 0
//  memset(packetBuffer, 0, NTP_PACKET_SIZE);
//  // Initialize values needed to form NTP request
//  // (see URL above for details on the packets)
//  //Serial.println("2");
//  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
//  packetBuffer[1] = 0;     // Stratum, or type of clock
//  packetBuffer[2] = 6;     // Polling Interval
//  packetBuffer[3] = 0xEC;  // Peer Clock Precision
//  // 8 bytes of zero for Root Delay & Root Dispersion
//  packetBuffer[12]  = 49;
//  packetBuffer[13]  = 0x4E;
//  packetBuffer[14]  = 49;
//  packetBuffer[15]  = 52;
//
//  //Serial.println("3");
//
//  // all NTP fields have been given values, now
//  // you can send a packet requesting a timestamp:
//  Udp.beginPacket(address, 123); //NTP requests are to port 123
//  //Serial.println("4");
//  Udp.write(packetBuffer, NTP_PACKET_SIZE);
//  //Serial.println("5");
//  Udp.endPacket();
//  //Serial.println("6");
//}

