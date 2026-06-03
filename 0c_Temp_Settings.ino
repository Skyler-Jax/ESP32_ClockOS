/////////////////////////////////////
//Function for Temperature Settings menu//
/////////////////////////////////////
void tempSettings() {

  /*Draw menu on main display*/
  if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
    oled1.clearDisplay();   //Clear display buffer for new frame
    oled1.setTextSize(3);
    oled1.setCursor(0, 0);
    oled1.print("Temp");
    oled1.drawLine(0, 25, 127, 25, SSD1306_WHITE);
    oled1.drawLine(0, 26, 127, 26, SSD1306_WHITE);
    oled1.setTextSize(1);
    oled1.setCursor(0, 32);
    oled1.print("A - °C/°F      ");
    if (tempCF == true) oled1.println("°C");
    else oled1.println("°F");
    oled1.print("B - Temp Comp  ");
    if (tempComp != 0) {
      oled1.print("°");
      oled1.println(tempComp);
    }
    oled1.display();    //Draw frame on display
  }

  /*Read keypad key press and process selection*/
  char key = keypad.getKey();
  if (key != NO_KEY) {
    delay(50);    //Debounce delay
    if (nightModeEnable == true && nightModeActive == true) { //Wake from night mode
      nightModeActivity();
      return loop();
    }
    else nightModeActivity();

    /*Toggle °C/°F*/
    if (key == 'A') {
      tempCF = !tempCF;
      EEPROM.writeBool(0xB0, tempCF);
      EEPROM.commit();
      if (buttonBeepEnable == true) singleBeep(20);
    }

    /*Set temperature compensation (°C)*/
    if (key == 'B') {
      if (buttonBeepEnable == true) singleBeep(20);
      setTempComp();
    }

    /*Return to Main Menu*/
    if (key == '#') {
      sysSettingsShow = false;
      menuShow = true;
      if (buttonBeepEnable == true) singleBeep(20);
    }
  }
}

////////////////////////////////
//Set Temperature Compensation//
////////////////////////////////
void setTempComp() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(0, 0);
  oled1.print("Enter Comp Value");
  oled1.setTextSize(1);
  oled1.setCursor(0,16);
  oled1.print("* for decimal");
  oled1.drawLine(0, 25, 127, 25, SSD1306_WHITE);
  oled1.drawLine(0, 26, 127, 26, SSD1306_WHITE);
  oled1.setTextSize(1);
  oled1.setTextSize(3);
  oled1.setCursor(0, 40);
  oled1.display();
  while (setBit == true) {
    runMasterClock();
    runTime();
    runTimer();
    if (timerRun == false) displayClock(2);
    else displayTimer(2);
    char key = keypad.getKey();
    if (key != NO_KEY) {
      delay(50);
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        oled1.print(key);
        oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '*') {
        char key = '.';
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        oled1.print(key);
        oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '#') {
        if (bufferIndex == 0) {
          setBit = false;
          if (infoChimeEnable == true) errorChime();
        } else if (atof(inputBuffer) < -10 || atof(inputBuffer) > 10) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setTempComp();
        } else {
          inputBuffer[bufferIndex] = '\0';
          tempComp = atof(inputBuffer);
          EEPROM.writeFloat(0xB1, atof(inputBuffer));
          EEPROM.commit();
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setBit = false;
          previousTempRefresh = 0;
          if (infoChimeEnable == true) successChime();
        }
      }
    }
  }
}