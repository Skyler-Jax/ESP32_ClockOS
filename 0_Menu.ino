//////////////////////////
//Function for main menu//
//////////////////////////
void menu() {

  /*draw menu on main display*/
  if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
    oled1.clearDisplay();   //Clear display buffer for new frame
    oled1.setTextSize(3);
    oled1.setCursor(0, 0);
    oled1.print("Menu");
    oled1.drawLine(0, 25, 127, 25, SSD1306_WHITE);
    oled1.drawLine(0, 26, 127, 26, SSD1306_WHITE);
    oled1.setTextSize(1);
    oled1.setCursor(0, 36);
    oled1.print("A - Clock   B - Timer");
    oled1.setCursor(0, 50);
    oled1.print("C - Temp    D - Radio");
    oled2.clearDisplay();
    oled2.setTextSize(3);
    oled2.setCursor(0, 0);
    oled2.print("Setting");
    oled2.drawLine(0, 25, 127, 25, SSD1306_WHITE);
    oled2.drawLine(0, 26, 127, 26, SSD1306_WHITE);
    oled2.setTextSize(1);
    oled2.setCursor(0, 36);
    oled2.print("1 - Systm   2 - Clock");
    oled2.setCursor(0, 50);
    oled2.print("3 - Temp    4 - Radio");
    dcInputIcon(onDC, 1, 101, 4);
    oled1.display();    //Draw frame on display 1
    oled2.display();    //Draw frame on display 2
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

    /*Enter system settings menu*/
    if (key == '1') {
      menuShow = false;
      sysSettingsShow = true;
      if (buttonBeepEnable == true) singleBeep(20);
    }

    /*Send selected option to flag setter*/
    if (key == 'A') menuOpts(1);
    if (key == 'B') menuOpts(2);
    if (key == 'C') menuOpts(3);
    if (key == 'D') menuOpts(4);

    /*Reset pages marked for subdisplay priority*/
    if (key == '*') {
      oled2Priority = false;
      oled2Page = 0;
      if (buttonBeepEnable == true) singleBeep(20);
    }

    /*Return to previous selection from menu if '#' pressed*/
    if (key == '#' && lastPage == 1) {
      menuShow = !menuShow;
      clockShow = true;
      if (buttonBeepEnable == true) singleBeep(50);
    } else if (key == '#' && lastPage == 2) {
      menuShow = !menuShow;
      timerShow = true;
      if (buttonBeepEnable == true) singleBeep(50);
    } else if (key == '#' && lastPage == 3) {
      menuShow = !menuShow;
      tempShow = true;
      if (buttonBeepEnable == true) singleBeep(50);
    } else if (key == '#' && lastPage == 4) {
      menuShow = !menuShow;
      radioShow = true;
      if (buttonBeepEnable == true) singleBeep(50);
    
    }
  }
}

///////////////////////////////////////////////
//Function sets flags based on menu selection//
///////////////////////////////////////////////
void menuOpts(int menuOpt) {
  if (menuOpt == 1) {
    resetCounters();
    clockShow = true;
    menuShow = false;
    lastPage = 1;
    if (oled2Page == 1 && oled2Priority == true) displayBlank(2);
    if (buttonBeepEnable == true) singleBeep(50);
  }
  if (menuOpt == 2) {
    resetCounters();
    timerShow = true;
    menuShow = false;
    lastPage = 2;
    if (oled2Page == 2 && oled2Priority == true) displayBlank(2);
    if (buttonBeepEnable == true) singleBeep(50);
  }
  if (menuOpt == 3) {
    resetCounters();
    tempShow = true;
    menuShow = false;
    lastPage = 3;
    if (oled2Page == 3 && oled2Priority == true) displayBlank(2);
    if (buttonBeepEnable == true) singleBeep(50);
  }
  if (menuOpt == 4) {
    resetCounters();
    radioShow = true;
    menuShow = false;
    lastPage = 4;
    if (oled2Page == 4 && oled2Priority == true) displayBlank(2);
    if (buttonBeepEnable == true) singleBeep(50);
  }
}
