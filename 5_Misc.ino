////////////////////
//Sound Generation//
////////////////////
void singleBeep(int beepDur) {
  digitalWrite(beeper, HIGH);
  delay(beepDur);
  digitalWrite(beeper, LOW);
}

void doubleBeep(int beepDur) {
  digitalWrite(beeper, HIGH);
  delay(beepDur);
  digitalWrite(beeper, LOW);
  delay(beepDur);
  digitalWrite(beeper, HIGH);
  delay(beepDur);
  digitalWrite(beeper, LOW);
  delay(beepDur);
}

void hourChime() {
  tone(chime, 3520, 50);
  delay(100);
  tone(chime, 2794, 100);
  delay(100);
  noTone(chime);
}

void setChime() {
  tone(chime, 4186);
  delay(20);
  tone(chime, 3136);
  delay(20);
  tone(chime, 2637);
  delay(20);
  tone(chime, 2093);
  delay(20);
  noTone(chime);
}

void successChime() {
  tone(chime, 2093);
  delay(20);
  tone(chime, 2637);
  delay(20);
  tone(chime, 3136);
  delay(20);
  tone(chime, 4186);
  delay(20);
  noTone(chime);
}

void errorChime() {
  tone(chime, 150);
  delay(10);
  tone(chime, 155);
  delay(10);
  tone(chime, 150);
  delay(10);
  tone(chime, 155);
  delay(10);
  tone(chime, 150);
  delay(10);
  tone(chime, 155);
  delay(10);
  tone(chime, 150);
  delay(10);
  tone(chime, 155);
  delay(10);
  tone(chime, 150);
  delay(10);
  tone(chime, 155);
  delay(10);
  tone(chime, 150);
  delay(10);
  noTone(chime);
}

///////////////////////
//Graphics generation//
///////////////////////
/*Power status icon on menu page*/
void dcInputIcon(bool onDC, int oledDisp, int posX, int posY) {
  if (oledDisp == 1) {
    if (onDC == false) oled1.drawBitmap(posX, posY, bmp_usb, 24, 14, WHITE);
    if (onDC == true) {
      if (battLvl == 5) oled1.drawBitmap(posX, posY, bmp_batt4, 24, 14, WHITE);
      if (battLvl == 4) oled1.drawBitmap(posX, posY, bmp_batt3, 24, 14, WHITE);
      if (battLvl == 3) oled1.drawBitmap(posX, posY, bmp_batt2, 24, 14, WHITE);
      if (battLvl == 2) oled1.drawBitmap(posX, posY, bmp_batt1, 24, 14, WHITE);
      if (battLvl == 1) oled1.drawBitmap(posX, posY, bmp_batt0, 24, 14, WHITE);
      if (battLvl == 0) oled1.drawBitmap(posX, posY, bmp_dc, 24, 14, WHITE);
    }
  }
  if (oledDisp == 2) {
    if (onDC == false) oled2.drawBitmap(posX, posY, bmp_usb, 24, 14, WHITE);
    if (onDC == true) {
      if (battLvl == 5) oled2.drawBitmap(posX, posY, bmp_batt4, 24, 14, WHITE);
      if (battLvl == 4) oled2.drawBitmap(posX, posY, bmp_batt3, 24, 14, WHITE);
      if (battLvl == 3) oled2.drawBitmap(posX, posY, bmp_batt2, 24, 14, WHITE);
      if (battLvl == 2) oled2.drawBitmap(posX, posY, bmp_batt1, 24, 14, WHITE);
      if (battLvl == 1) oled2.drawBitmap(posX, posY, bmp_batt0, 24, 14, WHITE);
      if (battLvl == 0) oled2.drawBitmap(posX, posY, bmp_dc, 24, 14, WHITE);
    }
  }
}

/*Timer active animation*///x101 y0
void drawTimerBMP(int timerBMP, int oledDisp, int posX, int posY) {
  if (oledDisp == 1) {
    if (timerBMP == 1) oled1.drawBitmap(posX, posY, bmp_timer1, 14, 24, WHITE);
    if (timerBMP == 2) oled1.drawBitmap(posX, posY, bmp_timer2, 14, 24, WHITE);
    if (timerBMP == 3) oled1.drawBitmap(posX, posY, bmp_timer3, 14, 24, WHITE);
    if (timerBMP == 4) oled1.drawBitmap(posX, posY, bmp_timer4, 14, 24, WHITE);
    if (timerBMP == 5) oled1.drawBitmap(posX, posY, bmp_timer5, 14, 24, WHITE);
    if (timerBMP == 0) oled1.drawBitmap(posX, posY, bmp_timer6, 14, 24, WHITE);
  }
  if (oledDisp == 2) {
    if (timerBMP == 1) oled2.drawBitmap(posX, posY, bmp_timer1, 14, 24, WHITE);
    if (timerBMP == 2) oled2.drawBitmap(posX, posY, bmp_timer2, 14, 24, WHITE);
    if (timerBMP == 3) oled2.drawBitmap(posX, posY, bmp_timer3, 14, 24, WHITE);
    if (timerBMP == 4) oled2.drawBitmap(posX, posY, bmp_timer4, 14, 24, WHITE);
    if (timerBMP == 5) oled2.drawBitmap(posX, posY, bmp_timer5, 14, 24, WHITE);
    if (timerBMP == 0) oled2.drawBitmap(posX, posY, bmp_timer6, 14, 24, WHITE);
  }
}

/*Audio mode icon*/
void audioModeIcon(int oledDisp, int posX, int posY) {
  if (oledDisp == 1) {
    if (audioMode == true) oled1.drawBitmap(posX, posY, bmp_Radio, 25, 19, WHITE);
    if (audioMode == false) oled1.drawBitmap(posX, posY, bmp_BT, 14, 19, WHITE);
  }
  if (oledDisp == 2) {
    if (audioMode == true) oled2.drawBitmap(posX, posY, bmp_Radio, 25, 19, WHITE);
    if (audioMode == false) oled2.drawBitmap(posX, posY, bmp_BT, 14, 19, WHITE);
  }
}

/*Radio signal strength bars*/
void signalLevel(int oledDisp, int posX, int posY) {
  short level = radio.getSignalLevel();
  if (oledDisp == 1) {
    oled1.drawBitmap(posX, posY, bmp_signalAntenna, 16, 16, WHITE);
    if (level >= 0 && level <= 3) {
      oled1.drawBitmap(posX + 16, posY, bmp_signal1, 3, 16, WHITE);
    }
    if (level >= 4 && level <= 7) {
      oled1.drawBitmap(posX + 16, posY, bmp_signal1, 3, 16, WHITE);
      oled1.drawBitmap(posX + 21, posY, bmp_signal2, 3, 16, WHITE);
    }
    if (level >= 8 && level <= 11) {
      oled1.drawBitmap(posX + 16, posY, bmp_signal1, 3, 16, WHITE);
      oled1.drawBitmap(posX + 21, posY, bmp_signal2, 3, 16, WHITE);
      oled1.drawBitmap(posX + 26, posY, bmp_signal3, 3, 16, WHITE);
    }
    if (level >= 12 && level <= 15) {
      oled1.drawBitmap(posX + 16, posY, bmp_signal1, 3, 16, WHITE);
      oled1.drawBitmap(posX + 21, posY, bmp_signal2, 3, 16, WHITE);
      oled1.drawBitmap(posX + 26, posY, bmp_signal3, 3, 16, WHITE);
      oled1.drawBitmap(posX + 31, posY, bmp_signal4, 3, 16, WHITE);
    }
  }
  if (oledDisp == 2) {
    oled2.drawBitmap(posX, posY, bmp_signalAntenna, 16, 16, WHITE);
    if (level >= 0 && level <= 3) {
      oled2.drawBitmap(posX + 16, posY, bmp_signal1, 3, 16, WHITE);
    }
    if (level >= 4 && level <= 7) {
      oled2.drawBitmap(posX + 16, posY, bmp_signal1, 3, 16, WHITE);
      oled2.drawBitmap(posX + 21, posY, bmp_signal2, 3, 16, WHITE);
    }
    if (level >= 8 && level <= 11) {
      oled2.drawBitmap(posX + 16, posY, bmp_signal1, 3, 16, WHITE);
      oled2.drawBitmap(posX + 21, posY, bmp_signal2, 3, 16, WHITE);
      oled2.drawBitmap(posX + 26, posY, bmp_signal3, 3, 16, WHITE);
      }
    if (level >= 12 && level <= 15) {
      oled2.drawBitmap(posX + 16, posY, bmp_signal1, 3, 16, WHITE);
      oled2.drawBitmap(posX + 21, posY, bmp_signal2, 3, 16, WHITE);
      oled2.drawBitmap(posX + 26, posY, bmp_signal3, 3, 16, WHITE);
      oled2.drawBitmap(posX + 31, posY, bmp_signal4, 3, 16, WHITE);
    }
  }
}

/*Stereo/mono icon*/
void stereoIcon(bool stereo, int oledDisp, int posX, int posY) {
  if (oledDisp == 1) {
    if (stereo == true) oled1.drawBitmap(posX, posY, bmp_stereo, 40, 16, WHITE);
    else oled1.drawBitmap(posX, posY, bmp_mono, 18, 16, WHITE);
  }
  if (oledDisp == 2) {
    if (stereo == true) oled2.drawBitmap(posX, posY, bmp_stereo, 40, 16, WHITE);
    else oled2.drawBitmap(posX, posY, bmp_mono, 18, 16, WHITE);
  }
}

/////////////////////////
//Timekeeping Functions//
/////////////////////////
void runMasterClock() {
	masterClock = esp_timer_get_time() / 1000;
}

void runTime() {
  if (masterClock - previousTimeCount >= timeInterval || previousTimeRefresh == 0) {
    previousTimeCount = masterClock;
    DoW = RTC.getDoW();
    hour = RTC.getHour(h12, AMPM);
    minute = RTC.getMinute();
    second = RTC.getSecond();
    day = RTC.getDate();
    month = RTC.getMonth(centuryBit);
    year = RTC.getYear();
    if (hour != prevHour) {
      if (hourChimeEnable == true) hourChime();
      prevHour = hour;
    }
  }
}

void resetCounters() {
  previousTimeRefresh = 0;
  previousTimerRefresh = 0;
  previousTempRefresh = 0;
  previousRadioRefresh = 0;
}

void nightModeTimer() {
  int lightLevel = analogRead(ldr);
  if (nightModeTime > 0) {
    if (masterClock - previousNightModeCount >= timerInterval || previousNightModeCount == 0) {
      previousNightModeCount = masterClock;
      nightModeTime--;
    }
  }
  // if (nightModeTime == 0 && lightLevel < 76) {
  //   nightModeActive = true;
  //   displayBlank(1);
  //   displayBlank(2);
  // }
}

void nightModeActivity() {
  nightModeActive = false;
  resetCounters();
  previousNightModeCount = 0;
  nightModeTime = 6;
}

void runTimer() {
  if (timerRun == true) {
    if (masterClock - previousTimerCount >= timerInterval) {
      previousTimerCount = masterClock;
      if (timerHour > 0 || timerMin > 0 || timerSec > 0) {
        if (timerBMP == 5) timerBMP = 1;
        else if (timerBMP > 0) timerBMP++;
        if (timerSec > 0) {
          timerSec--;
        } else {
          if (timerMin > 0) {
            timerMin--;
            timerSec = 59;
          } else {
            if (timerHour > 0) {
              timerHour--;
              timerMin = 59;
              timerSec = 59;
            }
          }
        }
      }else if (timerHour == 0 && timerMin == 0 && timerSec == 0) {
        timerBMP=0;
        if (oled2Page == 2) {
          displayTimer(2, true);
          oled2.invertDisplay(true);
        } else {
          displayTimer(1, true);
          oled1.invertDisplay(true);
        }
        if (infoChimeEnable == true) doubleBeep(50);
        if (oled2Page == 2) oled2.invertDisplay(false);
        else oled1.invertDisplay(false);
      }
    }
  }
}

void resetTimer(int option) {
  if (option == 1) {
    timerBMP = 1;
    timerRun = !timerRun;
    previousTimerCount = 0;
    previousTimerRefresh = 0;
    if (buttonBeepEnable == true) singleBeep(20);
  }
  if (option == 2) {
    timerRun = false;
    timerHour = 0;
    timerMin = 0;
    timerSec = 0;
    previousTimerCount = 0;
    previousTimerRefresh = 0;
    if (buttonBeepEnable == true) successChime();
  }
}

//////////////////
//Misc functions//
//////////////////
/*Bluetooth Metadata*/
void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
  Serial.printf("==> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);
  if (id == ESP_AVRC_MD_ATTR_PLAYING_TIME) {
    uint32_t playtime = String((char*)text).toInt();
    Serial.printf("==> Playing time is %d ms (%d seconds)\n", playtime, (int)round(playtime/1000.0));
  }
}

/*Bluetooth playback status*/
void getBTPlayStatus(esp_avrc_playback_stat_t playback) {
  switch (playback) {
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_STOPPED:
      playStatus = "Stop";
      break;
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PLAYING:
      playStatus = "Play";
      break;
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PAUSED:
      playStatus = "Pause";
      break;
  }
}

/*Determine battery level*/
void battLevel(int battChk) {
  if (battChk == 0) battLvl = 0;
  if (battChk != 0) {
    if (battChk > 1010) battLvl = 5;
    if (battChk < 1010) battLvl = 4;
    if (battChk < 960) battLvl = 3;
    if (battChk < 860) battLvl = 2;
    if (battChk < 820) battLvl = 1;
  } 
}

/*Read keypad*/
char getKeyChar() {
  char key = keyPad.getChar();
  return key;
}

/*Display Blanking*/
void displayBlank(int oledDisp) {
  if (oledDisp == 1) {
    oled1.clearDisplay();
    oled1.display();
  }
  if (oledDisp == 2) {
    oled2.clearDisplay();
    oled2.display();
  }
}