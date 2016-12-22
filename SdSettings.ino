void setupSdCard() {


  didReadConfig = false;
  hello = 0;
  doDelay = false;
  waitMs = 0;


  // Setup the SD card
 
  Serial.println("Calling SD.begin()...");
  if (!SD.begin(pinSelectSD)) {
    Serial.println("SD.begin() failed. Check: ");
    Serial.println("  card insertion,");
    Serial.println("  SD shield I/O pins and chip select,");
    Serial.println("  card formatting.");
    return;
  }
  Serial.println("...succeeded.");

  // Read our configuration from the SD card file.
  delay(2000);
  // didReadConfig = readConfiguration();
}

boolean readConfiguration() {
  
  /*
     Length of the longest line expected in the config file.
     The larger this number, the more memory is used
     to read the file.
     You probably won't need to change this number.
  */
  const uint8_t CONFIG_LINE_LENGTH = 127;

  // The open configuration file.
  SDConfigFile cfg;

  // Open the configuration file.
  if (!cfg.begin(CONFIG_FILE, CONFIG_LINE_LENGTH)) {
    Serial.print("Failed to open configuration file: ");
    Serial.println(CONFIG_FILE);
    return false;
  }

  // Read each setting from the file.
  while (cfg.readNextSetting()) {

    // Put a nameIs() block here for each setting you have.

    // doDelay
    if (cfg.nameIs("doDelay")) {

      doDelay = cfg.getBooleanValue();
      Serial.print("Read doDelay: ");
      if (doDelay) {
        Serial.println("true");
      } else {
        Serial.println("false");
      }

      // waitMs integer
    } else if (cfg.nameIs("waitMs")) {

      waitMs = cfg.getIntValue();
      Serial.print("Read waitMs: ");
      Serial.println(waitMs);

      // hello string (char *)
    }

    // General settings

    else if (cfg.nameIs("numberOfHives")) {
      hives = cfg.getIntValue();
      Serial.print("Numbers of hives: ");
      Serial.println(hives);
    }
    else if (cfg.nameIs("sensorsPerHive")) {
      sensorsPerHive = cfg.getIntValue();
      Serial.print("Numbers of temperature sensors per hive: ");
      Serial.println(sensorsPerHive);
    }
    else if (cfg.nameIs("scalesConnected")) {
      scale = cfg.getBooleanValue();
      Serial.print("Read doDelay: ");
      if (scale) {
        Serial.println("yes");
      } else {
        Serial.println("no");
      }
    }
    else if (cfg.nameIs("weatherStationConnected")) {
      weatherStation = cfg.getBooleanValue();
      Serial.print("Read doDelay: ");
      if (weatherStation) {
        Serial.println("yes");
      } else {
        Serial.println("no");
      }
    }

    // connectivity settings

    else if (cfg.nameIs("wifi")) {
      useWifi = cfg.getBooleanValue();
      Serial.print("Use wifi: ");
      if (useWifi) {
        Serial.println("yes");
      } else {
        Serial.println("no");
      }
    }
    else if (cfg.nameIs("ssid")) {
      ssid = cfg.copyValue();
      Serial.print("ssid: ");
      Serial.println(ssid);
    }
    else if (cfg.nameIs("pass")) {
      pass = cfg.copyValue();
      Serial.print("pass: ");
      Serial.println(pass);
    }
    else if (cfg.nameIs("gprs")) {
      useGprs = cfg.getBooleanValue();
      Serial.print("Use gprs: ");
      if (useGprs) {
        Serial.println("yes");
      } else {
        Serial.println("no");
      }
    }

    // hive specific settings
    else if (cfg.nameIs("publicKey1")) {
      publicKey[0] = cfg.copyValue();
      Serial.print("publicKey1: ");
      Serial.println(publicKey[0]);
    }
    else if (cfg.nameIs("privateKey1")) {
      privateKey[0] = cfg.copyValue();
      Serial.print("privateKey1: ");
      Serial.println(privateKey[0]);
    }
    else if (cfg.nameIs("publicKey2")) {
      publicKey[1] = cfg.copyValue();
      Serial.print("publicKey2: ");
      Serial.println(publicKey[1]);
    }
    else if (cfg.nameIs("privateKey2")) {
      privateKey[1] = cfg.copyValue();
      Serial.print("privateKey2: ");
      Serial.println(privateKey[1]);
    }
    else if (cfg.nameIs("publicKey3")) {
      publicKey[2] = cfg.copyValue();
      Serial.print("publicKey3: ");
      Serial.println(publicKey[2]);
    }
    else if (cfg.nameIs("privateKey3")) {
      privateKey[2] = cfg.copyValue();
      Serial.print("privateKey3: ");
      Serial.println(privateKey[2]);
    }
    //    else if (cfg.nameIs("publicKey4")) {
    //      publicKey[3] = cfg.copyValue();
    //      Serial.print("publicKey4: ");
    //      Serial.println(publicKey[3]);
    //    }
    //    else if (cfg.nameIs("privateKey4")) {
    //      privateKey[3] = cfg.copyValue();
    //      Serial.print("privateKey4: ");
    //      Serial.println(privateKey[3]);
    //    }
    //    else if (cfg.nameIs("publicKey5")) {
    //      publicKey[4] = cfg.copyValue();
    //      Serial.print("publicKey5: ");
    //      Serial.println(publicKey[4]);
    //    }
    //    else if (cfg.nameIs("privateKey5")) {
    //      // Dynamically allocate a copy of the string.
    //      privateKey[4] = cfg.copyValue();
    //      Serial.print("privateKey5: ");
    //      Serial.println(privateKey[4]);
    //    }



    else {
      // report unrecognized names.
      Serial.print("Unknown name in config: ");
      Serial.println(cfg.getName());
    }
  }

  // clean up
  cfg.end();
}
