///////////////////////////////////////////////////////
//Function for setting the time in the hardware clock//
///////////////////////////////////////////////////////
void setClock() {
  getDoW();
  getHour();
  getMin();
  getAMPM();
  getYear();
  getMonth();
  getDay();
  if (setAMPM == 2) {
    setHour = setHour + 12;
  } else if (setAMPM == 1 && setHour == 12) {
    setHour = 0;
  }
  if (setAMPM == 2) {
    AMPM = true;
  } else if (setAMPM == 1) {
    AMPM = false;
  }
  byte DoW = setDoW;
  byte Hour = setHour;
  byte Minute = setMin;
  byte Year = setYear;
  byte Month = setMonth;
  byte Day = setDay;
  if (confirmSave() == true) {
    RTC.setClockMode(true);
    RTC.setDoW(DoW);
    RTC.setHour(Hour);
    RTC.setMinute(Minute);
    RTC.setSecond(0);
    RTC.setYear(Year);
    RTC.setMonth(Month);
    RTC.setDate(Day);
  }
  setYear = 0;
  setMonth = 0;
  setDay = 0;
  setHour = 0;
  setMin = 0;
  setAMPM = 0;
  setDoW = 0;
}

////////////////////////////////////
//Get Day of Week to set from user//
////////////////////////////////////
void getDoW() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(22, 0);
  oled1.print("Weekday");
  oled1.drawLine(21, 17, 104, 17, SSD1306_WHITE);
  oled1.setTextSize(1);
  oled1.setCursor(7, 21);
  oled1.print("1-Su 2-Mo 3-Tu 4-We");
  oled1.setCursor(20, 30);
  oled1.print("5-Th 6-Fr 7-Sa");
  oled1.drawLine(0, 39, 127, 39, SSD1306_WHITE);
  oled1.drawLine(0, 40, 127, 40, SSD1306_WHITE);
  oled1.setTextSize(2);
  oled1.setCursor(0, 48);
  oled1.display();
  while (setBit == true) {
    runMasterClock();
    runTime();
    runTimer();
    if (timerRun == false) displayClock(2);
    else displayTimer(2);
    if (keyPad.isPressed()) {
      delay(50);
      char key = getKeyChar();
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        if (bufferIndex > 1) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getDoW();
        }
        if (key == '1' && bufferIndex <= 1) oled1.print("Sunday");
        if (key == '2' && bufferIndex <= 1) oled1.print("Monday");
        if (key == '3' && bufferIndex <= 1) oled1.print("Tuesday");
        if (key == '4' && bufferIndex <= 1) oled1.print("Wednesday");
        if (key == '5' && bufferIndex <= 1) oled1.print("Thursday");
        if (key == '6' && bufferIndex <= 1) oled1.print("Friday");
        if (key == '7' && bufferIndex <= 1) oled1.print("Sunday");
        if (nightModeActive == false) oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '#') {
        if (bufferIndex == 0) {
          setDoW = RTC.getDoW();
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        } else if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 7) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getDoW();
        } else {
          inputBuffer[bufferIndex] = '\0';
          setDoW = atoi(inputBuffer);
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        }
      }
    }
  }
}

/////////////////////////////
//Get hour to set from user//
/////////////////////////////
void getHour() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(40, 0);
  oled1.print("Hour");
  oled1.drawLine(39, 17, 86, 17, SSD1306_WHITE);
  oled1.setTextSize(1);
  oled1.setCursor(46, 21);
  oled1.print("1 - 12");
  oled1.drawLine(0, 31, 127, 31, SSD1306_WHITE);
  oled1.drawLine(0, 32, 127, 32, SSD1306_WHITE);
  oled1.setTextSize(3);
  oled1.setCursor(3, 40);
  oled1.display();
  while (setBit == true) {
    runMasterClock();
    runTime();
    runTimer();
    if (timerRun == false) displayClock(2);
    else displayTimer(2);
    if (keyPad.isPressed()) {
      delay(50);    //Debounce timer
      char key = getKeyChar();
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
        if (nightModeActive == false) oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '#') {
        if (bufferIndex == 0) {
          setHour = RTC.getHour(h12, AMPM);
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        } else if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 12) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getHour();
        } else {
          inputBuffer[bufferIndex] = '\0';
          setHour = atoi(inputBuffer);
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        }
      }
    }
  }
}

///////////////////////////////
//Get minute to set from user//
///////////////////////////////
void getMin() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(28, 0);
  oled1.print("Minute");
  oled1.drawLine(27, 17, 99, 17, SSD1306_WHITE);
  oled1.setTextSize(1);
  oled1.setCursor(46, 21);
  oled1.print("0 - 60");
  oled1.drawLine(0, 31, 127, 31, SSD1306_WHITE);
  oled1.drawLine(0, 32, 127, 32, SSD1306_WHITE);
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
      delay(50);    //Debounce timer
      char key = getKeyChar();
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        if (bufferIndex > 2) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getMin();
        }
        oled1.print(key);
        if (nightModeActive == false) oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '#') {
        if (bufferIndex == 0) {
          setMin = RTC.getMinute();
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        } else if (atoi(inputBuffer) > 59) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getMin();
        } else {
          inputBuffer[bufferIndex] = '\0';
          setMin = atoi(inputBuffer);
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        }
      }
    }
  }
}

///////////////////////////
// Get AM or PM from user//
///////////////////////////
void getAMPM() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(22, 0);
  oled1.print("AM / PM");
  oled1.drawLine(21, 17, 104, 17, SSD1306_WHITE);
  oled1.setTextSize(1);
  oled1.setCursor(37, 21);
  oled1.print("1-AM 2-PM");
  oled1.drawLine(0, 31, 127, 31, SSD1306_WHITE);
  oled1.drawLine(0, 32, 127, 32, SSD1306_WHITE);
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
      delay(50);    //Debounce timer
      char key = getKeyChar();
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        if (bufferIndex > 1) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getAMPM();
        }
        if (key == '1' && bufferIndex <= 1) oled1.print("AM");
        if (key == '2' && bufferIndex <= 1) oled1.print("PM");
        if (buttonBeepEnable == true) singleBeep(20);
        if (nightModeActive == false) oled1.display();
      }
      if (key == '#') {
        if (bufferIndex == 0) {
          if (AMPM == false) setAMPM = 1;
          if (AMPM == true) setAMPM = 2;
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        } else if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 2) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getAMPM();
        } else {
          inputBuffer[bufferIndex] = '\0';
          setAMPM = atoi(inputBuffer);
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        }
      }
    }
  }
}

/////////////////////////////
//Get year to set from user//
/////////////////////////////
void getYear() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(40, 0);
  oled1.print("Year");
  oled1.drawLine(39, 17, 86, 17, SSD1306_WHITE);
  oled1.setTextSize(1);
  oled1.setCursor(52, 21);
  oled1.print("20__");
  oled1.drawLine(0, 31, 127, 31, SSD1306_WHITE);
  oled1.drawLine(0, 32, 127, 32, SSD1306_WHITE);
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
      delay(50);    //Debounce timer
      char key = getKeyChar();
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        if (bufferIndex > 2) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getYear();
        }
        oled1.print(key);
        if (nightModeActive == false) oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '#') {
        if (bufferIndex == 0) {
          setYear = RTC.getYear();
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        } else if (atoi(inputBuffer) < 20 || atoi(inputBuffer) > 99) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getYear();
        } else {
          inputBuffer[bufferIndex] = '\0';
          setYear = atoi(inputBuffer);
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        }
      }
    }
  }
}

//////////////////////////////
//Get month to set from user//
//////////////////////////////
void getMonth() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(34, 0);
  oled1.print("Month");
  oled1.drawLine(33, 17, 93, 17, SSD1306_WHITE);
  oled1.setTextSize(1);
  oled1.setCursor(46, 21);
  oled1.print("1 - 12");
  oled1.drawLine(0, 31, 127, 31, SSD1306_WHITE);
  oled1.drawLine(0, 32, 127, 32, SSD1306_WHITE);
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
      delay(50);    //Debounce timer
      char key = getKeyChar();
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        if (bufferIndex > 2) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getMonth();
        }
        oled1.print(key);
        if (nightModeActive == false) oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
     if (key == '#') {
      if (bufferIndex == 0) {
        setMonth = RTC.getMonth(centuryBit);
        setBit = false;
        if (buttonBeepEnable == true) singleBeep(20);
      } else if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 12) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getMonth();
        } else {
          inputBuffer[bufferIndex] = '\0';
          setMonth = atoi(inputBuffer);
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        }
      }
    }
  }
}

/////////////////////////////////////
//Get day of month to set from user//
/////////////////////////////////////
void getDay() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(46, 0);
  oled1.print("Day");
  oled1.drawLine(45, 17, 81, 17, SSD1306_WHITE);
  oled1.setTextSize(1);
  oled1.setCursor(46, 21);
  oled1.print("1 - 12");
  oled1.drawLine(0, 31, 127, 31, SSD1306_WHITE);
  oled1.drawLine(0, 32, 127, 32, SSD1306_WHITE);
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
      delay(50);    //Debounce timer
      char key = getKeyChar();
      if (key >= '0' && key <= '9') {
        inputBuffer[bufferIndex] = key;
        bufferIndex++;
        if (bufferIndex > 2) {
          if (infoChimeEnable == true) errorChime();
          inputBuffer[bufferIndex] = '\0';
          bufferIndex = 0;
          memset(inputBuffer, 0, sizeof(inputBuffer));
          getDay();
        }
        oled1.print(key);
        if (nightModeActive == false) oled1.display();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '#') {
        if (bufferIndex == 0) {
          setDay = RTC.getDate();
          setBit = false;
          if (buttonBeepEnable == true) singleBeep(20);
        }
        if (atoi(inputBuffer) >= 1 && atoi(inputBuffer) <= 31) {
          if (setMonth == 1) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 31) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 2) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 28) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 3) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 31) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 4) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 30) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 5) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 31) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 6) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 30) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 7) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 31) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 8) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 31) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 9) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 30) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 10) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 31) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 11) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 30) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
          if (setMonth == 12) {
            if (atoi(inputBuffer) < 1 || atoi(inputBuffer) > 31) {
              if (infoChimeEnable == true) errorChime();
              inputBuffer[bufferIndex] = '\0';
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              getDay();
            } else {
              inputBuffer[bufferIndex] = '\0';
              setDay = atoi(inputBuffer);
              bufferIndex = 0;
              memset(inputBuffer, 0, sizeof(inputBuffer));
              setBit = false;
              if (buttonBeepEnable == true) singleBeep(20);
            }
          }
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////
//Confirms intended time set before writing to hardware clock//
///////////////////////////////////////////////////////////////
bool confirmSave() {
  setBit = true;
  oled1.clearDisplay();
  oled1.setTextSize(2);
  oled1.setCursor(22, 0);
  oled1.print("Confirm");
  oled1.drawLine(21, 17, 104, 17, SSD1306_WHITE);
  oled1.setCursor(0, 21);
  oled1.print("# - Save");
  oled1.setCursor(0, 40);
  oled1.print("* - Cancel");
  if (nightModeActive == false) oled1.display();
  while (setBit == true) {
    runMasterClock();
    runTime();
    runTimer();
    displayClock(2);
    if (keyPad.isPressed()) {
      delay(50);    //Debounce timer
      char key = getKeyChar();
      if (key == '#') {
        setBit = false;
        if (infoChimeEnable == true) successChime();
        return true;
      } else if (key == '*') {
        setYear = 0;
        setMonth = 0;
        setDay = 0;
        setHour = 0;
        setMin = 0;
        setAMPM = 0;
        setDoW = 0;
        setBit = false;
        if (infoChimeEnable == true) errorChime();
        return false;
      }
    }
  }
}
