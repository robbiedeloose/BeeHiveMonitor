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
  didReadConfig = readConfiguration();
}

boolean readConfiguration() {
  /*
   * Length of the longest line expected in the config file.
   * The larger this number, the more memory is used
   * to read the file.
   * You probably won't need to change this number.
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
    } else if (cfg.nameIs("ssid")) {
      
      // Dynamically allocate a copy of the string.
      ssid = cfg.copyValue();
      Serial.print("Read hello: ");
      Serial.println(ssid);

    } else {
      // report unrecognized names.
      Serial.print("Unknown name in config: ");
      Serial.println(cfg.getName());
    }
  }
  
  // clean up
  cfg.end();
}
