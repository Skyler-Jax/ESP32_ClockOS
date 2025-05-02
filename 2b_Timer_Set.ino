//////////////////////////////////
//Function for setting the timer//
//////////////////////////////////
void setTimer() {
  getTimerHour();
  getTimerMin();
  getTimerSec();
  if (nightModeEnable == true) nightModeActivity();
}

//////////////////////////////
//Get hours to set from user//
//////////////////////////////
void getTimerHour() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setCursor(0, 0);
  oled1.setTextSize(3);
  oled1.print("Hours");
  oled1.setCursor(0, 25);
  oled1.setTextSize(1);
  oled1.print("or # for zero");
  oled1.drawLine(0, 34, 127, 34, SSD1306_WHITE);
  oled1.drawLine(0, 35, 127, 35, SSD1306_WHITE);
  oled1.setTextSize(3);
  oled1.setCursor(2, 40);
  oled1.display();
  while (setBit == true) {
    runMasterClock();
    runTime();
    if (stby == false) displayRadio(2);
    else displayClock(2);
    char key = keypad.getKey();
    if (key != NO_KEY) {
      delay(50);
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        if (bufferIndex > 2) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getHour();
        }
        oled1.print(key);
        oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '#') {
        if (atoi(inputBuffer) > 12) {
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          if (infoChimeEnable == true) errorChime();
          getTimerHour();
        } else {
          inputBuffer[bufferIndex] = '\0';
          timerHour = atoi(inputBuffer);
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          Serial.println(timerHour);
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
          return;
        }        
      }
    }
  }
}

////////////////////////////////
//Get minutes to set from user//
////////////////////////////////
void getTimerMin() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setCursor(0, 0);
  oled1.setTextSize(3);
  oled1.print("Minutes");
  oled1.setCursor(0, 25);
  oled1.setTextSize(1);
  oled1.print("or # for zero");
  oled1.drawLine(0, 34, 127, 34, SSD1306_WHITE);
  oled1.drawLine(0, 35, 127, 35, SSD1306_WHITE);
  oled1.setTextSize(3);
  oled1.setCursor(2, 40);
  oled1.display();
  while (setBit == true) {
    runMasterClock();
    runTime();
    if (stby == false) displayRadio(2);
    else displayClock(2);
    char key = keypad.getKey();
    if (key != NO_KEY) {
      delay(50);
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        if (bufferIndex > 2) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getHour();
        }
        oled1.print(key);
        oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '#') {
        if (atoi(inputBuffer) > 59) {
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          if (infoChimeEnable == true) errorChime();
          getTimerMin();
        } else {
          inputBuffer[bufferIndex] = '\0';
          timerMin = atoi(inputBuffer);
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          Serial.println(timerMin);
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
          return;
        }        
      }
    }
  }
}

////////////////////////////////
//Get seconds to set from user//
////////////////////////////////
void getTimerSec() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setCursor(0, 0);
  oled1.setTextSize(3);
  oled1.print("Seconds");
  oled1.setCursor(0, 25);
  oled1.setTextSize(1);
  oled1.print("or # for zero");
  oled1.drawLine(0, 34, 127, 34, SSD1306_WHITE);
  oled1.drawLine(0, 35, 127, 35, SSD1306_WHITE);
  oled1.setTextSize(3);
  oled1.setCursor(2, 40);
  oled1.display();
  while (setBit == true) {
    runMasterClock();
    runTime();
    if (stby == false) displayRadio(2);
    else displayClock(2);
    char key = keypad.getKey();
    if (key != NO_KEY) {
      delay(50);
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        if (bufferIndex > 2) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getHour();
        }
        oled1.print(key);
        oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '#') {
        if (atoi(inputBuffer) > 59) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getTimerSec();
        } else {
          inputBuffer[bufferIndex] = '\0';
          timerSec = atoi(inputBuffer);
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          Serial.println(timerSec);
          setBit = false;
          if (infoChimeEnable == true) successChime();
          return;
        }        
      }
    }
  }
}
