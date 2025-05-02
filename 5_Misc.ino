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
  } else if (nightModeTime == 0 && lightLevel < 128) {
    nightModeActive = true;
    displayBlank(1);
    displayBlank(2);
  }
}

void nightModeActivity() {
  resetCounters();
  previousNightModeCount = 0;
  nightModeTime = 15;
	nightModeActive = false;
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
        if (lastPage != 2) {
          timerShow = true;
          lastPage = 2;
        }
        displayTimer(1, true);
        oled1.invertDisplay(true);
        if (infoChimeEnable == true) doubleBeep(50);
        oled1.invertDisplay(false);        
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
