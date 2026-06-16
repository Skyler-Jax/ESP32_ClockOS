////////////////////////////////////////
//Function for setting radio frequency//
////////////////////////////////////////
void setRadio() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(0, 0);
  oled1.print("Enter Freq");
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
    if (keyPad.isPressed()) {
      char key = getKeyChar();
      delay(150);    //Debounce delay
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
        } else if (atof(inputBuffer) <= 87.9 || atof(inputBuffer) >= 108.1) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setRadio();
        } else {
          inputBuffer[bufferIndex] = '\0';
          frequency = atof(inputBuffer);
          EEPROM.writeFloat(0x00, atof(inputBuffer));
          EEPROM.commit();
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setBit = 0;
          previousRadioRefresh = 0;
          if (infoChimeEnable == true) successChime();
        }
      }
    }
  }
}

//////////////////////////////////////////////
//Functions for toggling radio chip settings//
//////////////////////////////////////////////
void setStandby(bool stby) {
  radio.setStandby(stby);
}
void setStereoNC(bool snc) {
  radio.setStereoNC(snc);
}
void setMuted(bool muted) {
  radio.setMuted(muted);
}
