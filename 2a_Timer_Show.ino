///////////////////////////////////
//Function for timer display page//
///////////////////////////////////
void displayTimer(int oledDisp, bool oledOverride) {

  /*Code for primary OLED display*/
  if (oledDisp == 1) {

    /*Run timer page refresh timer*/
    if (masterClock - previousTimerRefresh >= timerInterval || previousTimerRefresh == 0) {
      previousTimerRefresh = masterClock;

      /*Draw timer on page display*/
      if (oledOverride == true || nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled1.clearDisplay();   //Clear display buffer for new frame
        oled1.setCursor(0, 0);
        oled1.setTextSize(3);
        oled1.print("Timer");
        oled1.drawLine(0, 25, 127, 25, SSD1306_WHITE);
        oled1.drawLine(0, 26, 127, 26, SSD1306_WHITE);
        if (timerRun == true) drawTimerBMP(timerBMP, 1, 101, 0);
        drawTimerCount(1);
        oled1.display();    //Draw frame on display
      }
    }

    /*Read keypad key press and process selection*/
    if (keyPad.isPressed()) {
      char key = getKeyChar();
      delay(150);    //Debounce delay
      if (nightModeEnable == true && nightModeActive == true) { //Wake from night mode
        nightModeActivity();
        return loop();
      }
      else nightModeActivity();

      /*Enter timer set mode*/
      if (key == '*') {
        previousTimerRefresh = 0;
        if (infoChimeEnable == true) setChime();
        setTimer();
      }

      /*Return to Main Menu*/
      if (key == '#') {
        timerShow = false;
        menuShow = true;
        lastPage = 0;
        resetCounters();
        if (buttonBeepEnable == true) singleBeep(50);
      }

      /*Start/stop timer*/
      if (key == 'A') {
        oledOverride = 0;
        resetTimer(1);
      }

      /*Reset timer*/
      if (key == 'B') resetTimer(2);

      /*Toggle current page for subdisplay priority*/
      if (key == 'D') {
        if (oled2Priority == true && oled2Page == 2) oled2Priority = 0;
        else {
          oled2Priority = 2;
          displayBlank(1);
          menuShow = true;
          lastPage = 0;
          resetCounters();
        }
        if (buttonBeepEnable == true) singleBeep(20);
      }
    }
  }

  /*Code for secondary OLED 'subdisplay'*/
  if (oledDisp == 2) {

    /*Run timer page refresh timer*/
    if (masterClock - previousTimerRefresh >= timerInterval || previousTimerRefresh == 0) {
      previousTimerRefresh = masterClock;

      /*Draw timer on page display*/
      if (oledOverride == true || nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled2.clearDisplay();   //Clear display buffer for new frame
        oled2.setCursor(0, 0);
        oled2.setTextSize(3);
        oled2.print("Timer");
        oled2.drawLine(0, 25, 127, 25, SSD1306_WHITE);
        oled2.drawLine(0, 26, 127, 26, SSD1306_WHITE);
        if (timerRun == true) drawTimerBMP(timerBMP, 2, 101, 0);
        drawTimerCount(2);
        oled2.display();    //Draw frame on display
      }
    }

    /*Read keypad key press and process selection*/
    if (keyPad.isPressed()) {
      char key = getKeyChar();
      delay(150);    //Debounce delay
      if (nightModeEnable == true && nightModeActive == true) { //Wake from night mode
        nightModeActivity();
        return loop();
      }
      else nightModeActivity();

      /*Stop timer when timer active on subdisplay*/
      if (key == 'A') {
        oledOverride = 0;
        singleBeep(20);
        resetTimer(1);
      }

      /*Stop and reset timer when timer active on subdisplay*/
      if (key == 'B') {
        singleBeep(20);
        resetTimer(2);
      }

      /*Return to main menu*/
      if (key == '#') {
        if (lastPage == 1) {
          clockShow = !clockShow;
          if (buttonBeepEnable == true) singleBeep(50);
          } else if (lastPage == 2) {
          timerShow = !timerShow;
          if (buttonBeepEnable == true) singleBeep(50);
          } else if (lastPage == 3) {
          tempShow = !tempShow;
          if (buttonBeepEnable == true) singleBeep(50);
          } else if (lastPage == 4) {
          audioShow = !audioShow;
          if (buttonBeepEnable == true) singleBeep(50);
        }
        menuShow = true;
        resetCounters();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////
//Function for getting current count from timer subroutine and updating page display//
//////////////////////////////////////////////////////////////////////////////////////
void drawTimerCount(int oledDisp) {
  if (oledDisp == 1) {
    oled1.setCursor(2, 43);
    oled1.setTextSize(3);
    if (timerHour < 10) oled1.print("0");
    oled1.print(timerHour);
    oled1.print(":");
    if (timerMin < 10) oled1.print("0");
    oled1.print(timerMin);
    oled1.setCursor(90, 41);
    oled1.setTextSize(2);
    oled1.print(":");
    if (timerSec < 10) oled1.print("0");
    oled1.print(timerSec);
  }
  if (oledDisp == 2) {
    oled2.setCursor(2, 43);
    oled2.setTextSize(3);
    if (timerHour < 10) oled2.print("0");
    oled2.print(timerHour);
    oled2.print(":");
    if (timerMin < 10) oled2.print("0");
    oled2.print(timerMin);
    oled2.setCursor(90, 41);
    oled2.setTextSize(2);
    oled2.print(":");
    if (timerSec < 10) oled2.print("0");
    oled2.print(timerSec);
  }
}
