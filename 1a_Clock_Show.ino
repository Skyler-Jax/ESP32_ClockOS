///////////////////////////////////
//Function for clock display page//
///////////////////////////////////
void displayClock(int oledDisp) {

  /*Code for primary OLED display*/
  if (oledDisp == 1) {

    /*Run clock page refresh timer*/
    if (masterClock - previousTimeRefresh >= timeInterval || previousTimeRefresh == 0) {
      previousTimeRefresh = masterClock;

      /*Draw clock page on display*/
      if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled1.clearDisplay();   //Clear display buffer for new frame

        /*Print the day of the week*/
          if (DoW == 1) {
          oled1.setCursor(0, 0);
          oled1.setTextSize(2);
          oled1.print("Sunday");
        }
        if (DoW == 2) {
          oled1.setCursor(0, 0);
          oled1.setTextSize(2);
          oled1.print("Monday");
        }
        if (DoW == 3) {
          oled1.setCursor(0, 0);
          oled1.setTextSize(2);
          oled1.print("Tuesday");
        }
        if (DoW == 4) {
          oled1.setCursor(0, 0);
          oled1.setTextSize(2);
          oled1.print("Wed'sday");
        }
        if (DoW == 5) {
          oled1.setCursor(0, 0);
          oled1.setTextSize(2);
          oled1.print("Thursday");
        }
        if (DoW == 6) {
          oled1.setCursor(0, 0);
          oled1.setTextSize(2);
          oled1.print("Friday");
        }
        if (DoW == 7) {
          oled1.setCursor(0, 0);
          oled1.setTextSize(2);
          oled1.print("Saturday");
        }

        /*Print the date*/
        oled1.setCursor(5, 18);
        oled1.setTextSize(2);
        if (month < 10) oled1.print("0");
        oled1.print(month);
        oled1.print("/");
        if (day < 10) oled1.print("0");
        oled1.print(day);
        oled1.print("/20");
        oled1.println(year);

        dcInputIcon(onDC, oledDisp, 101, 1);

        /*Seperate date and time sections with a horizontal line*/
        oled1.drawLine(0, 36, 127, 36, SSD1306_WHITE);
        oled1.drawLine(0, 37, 127, 37, SSD1306_WHITE);

        /*Print the time*/
        oled1.setCursor(2, 43);
        oled1.setTextSize(3);
        if (hour < 10) oled1.print("0");
        oled1.print(hour);
        oled1.print(":");
        if (minute < 10) oled1.print("0");
        oled1.print(minute);
        oled1.setCursor(90, 41);
        oled1.setTextSize(2);
        oled1.print(":");
        if (second < 10) oled1.print("0");
        oled1.print(second);
        if (AMPM == false) {
          oled1.setCursor(101, 57);
          oled1.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          oled1.setTextSize(1);
          oled1.print(" AM ");
          oled1.setTextColor(SSD1306_WHITE);
        }
        if (AMPM == true) {
          oled1.setCursor(101, 57);
          oled1.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          oled1.setTextSize(1);
          oled1.print(" PM ");
          oled1.setTextColor(SSD1306_WHITE);
        }
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

      /*Enter clock set mode*/
      if (key == '*') {
        previousTimeRefresh = 0;
        if (infoChimeEnable == true) setChime();
        setClock();
      }

      /*Return to Main Menu*/
      if (key == '#') {
        clockShow = false;
        menuShow = true;
        resetCounters();
        if (buttonBeepEnable == true) singleBeep(50);
      }

      /*Toggle current page for subdisplay priority*/
      if (key == 'D') {
        if (oled2Priority == true && oled2Page == 1) oled2Priority = 0;
        else {
          oled2Priority = 1;
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

    /*Run clock page refresh timer*/
    if (masterClock - previousTimeRefresh >= timeInterval || previousTimeRefresh == 0) {
      previousTimeRefresh = masterClock;

      /*Draw clock page on display*/
      if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled2.clearDisplay();   //Clear display buffer for new frame

        /*Print the day of the week*/
          if (DoW == 1) {
          if (menuShow == true) oled2.setCursor(28, 0);
          else oled2.setCursor(0, 0);
          oled2.setTextSize(2);
          oled2.print("Sunday");
        }
        if (DoW == 2) {
          if (menuShow == true) oled2.setCursor(28, 0);
          else oled2.setCursor(0, 0);
          oled2.setTextSize(2);
          oled2.print("Monday");
        }
        if (DoW == 3) {
          if (menuShow == true) oled2.setCursor(21, 0);
          else oled2.setCursor(0, 0);
          oled2.setTextSize(2);
          oled2.print("Tuesday");
        }
        if (DoW == 4) {
          if (menuShow == true) oled2.setCursor(10, 0);
          else oled2.setCursor(0, 0);
          oled2.setTextSize(2);
          if (menuShow == true) oled2.print("Wednesday");
          else oled2.print("Wed'sday");
        }
        if (DoW == 5) {
          if (menuShow == true) oled2.setCursor(16, 0);
          else oled2.setCursor(0, 0);
          oled2.setTextSize(2);
          oled2.print("Thursday");
        }
        if (DoW == 6) {
          if (menuShow == true) oled2.setCursor(28, 0);
          else oled2.setCursor(0, 0);
          oled2.setTextSize(2);
          oled2.print("Friday");
        }
        if (DoW == 7) {
          if (menuShow == true) oled2.setCursor(16, 0);
          else oled2.setCursor(0, 0);
          oled2.setTextSize(2);
          oled2.print("Saturday");
        }

        /*Print the date*/
        oled2.setCursor(5, 18);
        oled2.setTextSize(2);
        if (month < 10) oled2.print("0");
        oled2.print(month);
        oled2.print("/");
        if (day < 10) oled2.print("0");
        oled2.print(day);
        oled2.print("/20");
        oled2.println(year);

        dcInputIcon(onDC, oledDisp, 101, 1);

        /*Seperate date and time sections with a horizontal line*/
        oled2.drawLine(0, 36, 127, 36, SSD1306_WHITE);
        oled2.drawLine(0, 37, 127, 37, SSD1306_WHITE);

        /*Print the time*/
        oled2.setCursor(2, 43);
        oled2.setTextSize(3);
        if (hour < 10) oled2.print("0");
        oled2.print(hour);
        oled2.print(":");
        if (minute < 10) oled2.print("0");
        oled2.print(minute);
        oled2.setCursor(90, 41);
        oled2.setTextSize(2);
        oled2.print(":");
        if (second < 10) oled2.print("0");
        oled2.print(second);
        if (AMPM == false) {
          oled2.setCursor(101, 57);
          oled2.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          oled2.setTextSize(1);
          oled2.print(" AM ");
          oled2.setTextColor(SSD1306_WHITE);
        }
        if (AMPM == true) {
          oled2.setCursor(101, 57);
          oled2.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          oled2.setTextSize(1);
          oled2.print(" PM ");
          oled2.setTextColor(SSD1306_WHITE);
        }
        oled2.display();    //Draw frame on display
      }
    }
  }
}
