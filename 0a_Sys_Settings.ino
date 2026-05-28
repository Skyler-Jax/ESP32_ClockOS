/////////////////////////////////////
//Function for System Settings menu//
/////////////////////////////////////
void sysSettings() {

  /*Draw menu on main display*/
  if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
    oled1.clearDisplay();   //Clear display buffer for new frame
    oled1.setTextSize(3);
    oled1.setCursor(0, 0);
    oled1.print("Setting");
    oled1.drawLine(0, 25, 127, 25, SSD1306_WHITE);
    oled1.drawLine(0, 26, 127, 26, SSD1306_WHITE);
    oled1.setTextSize(1);
    oled1.setCursor(0, 32);
    oled1.print("A - Hour Chime ");
    if (hourChimeEnable == true) oled1.println("On");
    else oled1.println("Off");
    oled1.print("B - Info Chime ");
    if (infoChimeEnable == true) oled1.println("On");
    else oled1.println("Off");
    oled1.print("C - Button Beep ");
    if (buttonBeepEnable == true) oled1.println("On");
    else oled1.println("Off");
    oled1.print("D - Night Mode ");
    if (nightModeEnable == true) oled1.println("On");
    else oled1.println("Off");
    oled1.display();    //Draw frame on display
  }

  /*Read keypad key press and process selection*/
  char key = keypad.getKey();
  if (key != NO_KEY) {
    delay(50);    //Debounce delay
    if (nightModeEnable == true) nightModeActivity();    //Wake from night mode

    /*Toggle hour chime*/
    if (key == 'A') {
      hourChimeEnable = !hourChimeEnable;
      EEPROM.writeBool(0xA0, hourChimeEnable);
      EEPROM.commit();
      if (buttonBeepEnable == true) singleBeep(20);
    }

    /*Toggle button beep*/
    if (key == 'B') {
      infoChimeEnable = !infoChimeEnable;
      EEPROM.writeBool(0xA1, infoChimeEnable);
      EEPROM.commit();
      if (buttonBeepEnable == true) singleBeep(20);
    }

    /*Toggle info chime*/
    if (key == 'C') {
      buttonBeepEnable = !buttonBeepEnable;
      EEPROM.writeBool(0xA2, buttonBeepEnable);
      EEPROM.commit();
      if (buttonBeepEnable == true) singleBeep(20);
    }

    /*Toggle Night Mode display timeout*/
    if (key == 'D') {
      nightModeEnable = !nightModeEnable;
      EEPROM.writeBool(0xA3, nightModeEnable);
      EEPROM.commit();
      if (buttonBeepEnable == true) singleBeep(20);
    }

    /*Return to Main Menu*/
    if (key == '#') {
      sysSettingsShow = false;
      menuShow = true;
      if (buttonBeepEnable == true) singleBeep(20);
    }
  }
}
