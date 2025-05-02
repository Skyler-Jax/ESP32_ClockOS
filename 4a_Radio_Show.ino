///////////////////////////////////
//Function for radio display page//
///////////////////////////////////
void displayRadio(int oledDisp) {

  /*Code for primary OLED display*/
  if (oledDisp == 1) {

    /*Run clock page refresh timer*/
    if (masterClock - previousRadioRefresh >= radioInterval || previousRadioRefresh == 0) {
      previousRadioRefresh = masterClock;

      /*Update radio chip with current settings*/
      radio.setFrequency(frequency);
      radio.setStandby(stby);
      radio.setStereoNC(snc);
      radio.setMuted(muted);

      /*Check for stereo signal and process associated options*/
      bool stereo = radio.isStereo();
      if (stereo == true && sncDisable == false) snc = true;
      else if (stereo == true && sncDisable == true) snc = false;

      /*Draw radio on page display*/
      if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled1.clearDisplay();   //Clear display buffer for new frame
        if (stby == true) {
          oled1.setCursor(48, 0);
          oled1.setTextSize(1);
          oled1.print("Radio Standby");
        } else if (stby == false && muted == true) {
          oled1.setCursor(50, 0);
          oled1.setTextSize(1);
          oled1.print("Output Muted");
        } else signalLevel(oledDisp);
        oled1.setTextSize(3);
        oled1.setCursor(2, 40);
        oled1.print(frequency, 1);
        oled1.setTextSize(1);
        oled1.print(" MHz");
        if (stereo == true && stby == false && muted == false) oled1.drawBitmap(58, 0, bmp_stereo, 40, 16, WHITE);
        else if (stereo == false && stby == false && muted == false) oled1.drawBitmap(58, 0, bmp_mono, 18, 16, WHITE);
        oled1.display();    //Draw frame on display
      }
    }

    /*Read keypad key press and process selection*/
    char key = keypad.getKey();
    if (key != NO_KEY){
      delay(50);//Debounce timer
      if (nightModeEnable == true) nightModeActivity();   //Wake from night mode

      /*Toggle radio chip standby mode*/
      if (key == 'A') {
        stby = !stby;
        previousRadioRefresh = 0;
        if (buttonBeepEnable == true) singleBeep(20);
        displayRadio(1);
      }

      /*Toggle audio mute*/
      if (key == 'B') {
        muted = !muted;
        previousRadioRefresh = 0;
        if (buttonBeepEnable == true) singleBeep(20);
        displayRadio(1);
      }

      /*Toggle stereo noise cancellation*/
      if (key == 'C') {
        snc = !snc;
        previousRadioRefresh = 0;
        if (buttonBeepEnable == true) singleBeep(20);
        displayRadio(1);
      }

      /*Enter radio set mode*/
      if (key == '*') {
        previousRadioRefresh = 0;
        if (infoChimeEnable == true) setChime();
        setRadio();
      }

      /*Return to Main Menu*/
      if (key == '#') {
        radioShow = false;
        menuShow = true;
        lastPage = 0;
        resetCounters();
        if (buttonBeepEnable == true) singleBeep(50);
      }

      /*Toggle current page for subdisplay priority*/
      if (key == 'D') {
        if (oled2Priority == true && oled2Page == 4) oled2Priority = !oled2Priority;
        else {
          oled2Priority = true;
          oled2Page = 4;
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
    if (masterClock - previousRadioRefresh >= radioInterval || previousRadioRefresh == 0) {
      previousRadioRefresh = masterClock;

      /*Update radio chip with current settings*/
      radio.setFrequency(frequency);
      radio.setStandby(stby);
      radio.setStereoNC(snc);
      radio.setMuted(muted);

      /*Check for stereo signal and process associated options*/
      bool stereo = radio.isStereo();
      if (stereo == true && sncDisable == false) snc = true;
      else if (stereo == true && sncDisable == true) snc = false;

      /*Draw radio on page display*/
      if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled2.clearDisplay();   //Clear display buffer for new frame
        if (stby == true) {
          oled2.setCursor(48, 0);
          oled2.setTextSize(1);
          oled2.print("Radio Standby");
        } else if (stby == false && muted == true) {
          oled2.setCursor(50, 0);
          oled2.setTextSize(1);
          oled2.print("Output Muted");
        } else signalLevel(oledDisp);
        oled2.setTextSize(3);
        oled2.setCursor(2, 40);
        oled2.print(frequency, 1);
        oled2.setTextSize(1);
        oled2.print(" MHz");
        if (stereo == true && stby == false && muted == false) oled2.drawBitmap(58, 0, bmp_stereo, 40, 16, WHITE);
        else if (stereo == false && stby == false && muted == false) oled2.drawBitmap(58, 0, bmp_mono, 18, 16, WHITE);
        oled2.display();    //Draw frame on display
      }
    }
  }  
}

///////////////////////////////////////////////////
//Function for drawing radio signal strength bars//
///////////////////////////////////////////////////
void signalLevel(int oledDisp) {
  short level = radio.getSignalLevel();
  if (oledDisp == 1) {
    oled1.drawBitmap(0, 0, bmp_signalAntenna, 16, 16, WHITE);
    if (level >= 0 && level <= 3) {
      oled1.drawBitmap(16, 0, bmp_signal1, 3, 16, WHITE);
    }
    if (level >= 4 && level <= 7) {
      oled1.drawBitmap(16, 0, bmp_signal1, 3, 16, WHITE);
      oled1.drawBitmap(21, 0, bmp_signal2, 3, 16, WHITE);
    }
    if (level >= 8 && level <= 11) {
      oled1.drawBitmap(16, 0, bmp_signal1, 3, 16, WHITE);
      oled1.drawBitmap(21, 0, bmp_signal2, 3, 16, WHITE);
      oled1.drawBitmap(26, 0, bmp_signal3, 3, 16, WHITE);
    }
    if (level >= 12 && level <= 15) {
      oled1.drawBitmap(16, 0, bmp_signal1, 3, 16, WHITE);
      oled1.drawBitmap(21, 0, bmp_signal2, 3, 16, WHITE);
      oled1.drawBitmap(26, 0, bmp_signal3, 3, 16, WHITE);
      oled1.drawBitmap(31, 0, bmp_signal4, 3, 16, WHITE);
    }
  }
  if (oledDisp == 2) {
    oled2.drawBitmap(0, 0, bmp_signalAntenna, 16, 16, WHITE);
    if (level >= 0 && level <= 3) {
      oled2.drawBitmap(16, 0, bmp_signal1, 3, 16, WHITE);
    }
    if (level >= 4 && level <= 7) {
      oled2.drawBitmap(16, 0, bmp_signal1, 3, 16, WHITE);
      oled2.drawBitmap(21, 0, bmp_signal2, 3, 16, WHITE);
    }
    if (level >= 8 && level <= 11) {
      oled2.drawBitmap(16, 0, bmp_signal1, 3, 16, WHITE);
      oled2.drawBitmap(21, 0, bmp_signal2, 3, 16, WHITE);
      oled2.drawBitmap(26, 0, bmp_signal3, 3, 16, WHITE);
      }
    if (level >= 12 && level <= 15) {
      oled2.drawBitmap(16, 0, bmp_signal1, 3, 16, WHITE);
      oled2.drawBitmap(21, 0, bmp_signal2, 3, 16, WHITE);
      oled2.drawBitmap(26, 0, bmp_signal3, 3, 16, WHITE);
      oled2.drawBitmap(31, 0, bmp_signal4, 3, 16, WHITE);
    }
  }
}
