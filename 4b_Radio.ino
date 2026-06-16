///////////////////////////
//Function for radio mode//
///////////////////////////
void modeRadio(int oledDisp) {

  /*Update radio chip with current settings*/
  radio.setFrequency(frequency);
  radio.setStandby(stbyRadio);
  radio.setStereoNC(snc);
  radio.setMuted(muted);


  /*Code for primary OLED display*/
  if (oledDisp == 1) {

    /*Run clock page refresh timer*/
    if (masterClock - previousRadioRefresh >= radioInterval || previousRadioRefresh == 0) {
      previousRadioRefresh = masterClock;

      // /*Update radio chip with current settings*/
      // radio.setFrequency(frequency);
      // radio.setStandby(stbyRadio);
      // radio.setStereoNC(snc);
      // radio.setMuted(muted);

      /*Check for stereo signal and process associated options*/
      bool stereo = radio.isStereo();
      if (stereo == true && sncDisable == false) snc = true;
      else if (stereo == true && sncDisable == true) snc = false;

      /*Draw radio on page display*/
      if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled1.clearDisplay();   //Clear display buffer for new frame
        audioModeIcon(oledDisp, 0, 0);
        if (stbyRadio == true) {
          oled1.setCursor(48, 0);
          oled1.setTextSize(1);
          oled1.print("Radio Standby");
        } else if (stbyRadio == false && muted == true) {
          oled1.setCursor(50, 0);
          oled1.setTextSize(1);
          oled1.print("Output Muted");
        } else signalLevel(oledDisp, 92, 0);
        oled1.setTextSize(3);
        oled1.setCursor(2, 40);
        oled1.print(frequency, 1);
        oled1.setTextSize(1);
        oled1.print(" MHz");
        if (stbyRadio == false && muted == false) stereoIcon(stereo, oledDisp, 40, 2);
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

      /*Toggle radio chip standby mode*/
      if (key == 'A') {
        stbyRadio = !stbyRadio;
        if (stbyBT != true) stbyBT = true;
        previousRadioRefresh = 0;
        if (buttonBeepEnable == true) singleBeep(20);
        displayAudio(1);
      }

      /*Toggle audio mute*/
      if (key == 'B') {
        muted = !muted;
        previousRadioRefresh = 0;
        if (buttonBeepEnable == true) singleBeep(20);
        displayAudio(1);
      }

      /*Toggle stereo noise cancellation*/
      if (key == 'C') {
        snc = !snc;
        previousRadioRefresh = 0;
        if (buttonBeepEnable == true) singleBeep(20);
        displayAudio(1);
      }

      /*Enter radio set mode*/
      if (key == '*') {
        previousRadioRefresh = 0;
        if (infoChimeEnable == true) setChime();
        setRadio();
      }

      /*Return to Main Menu*/
      if (key == '#') {
        audioShow = false;
        menuShow = true;
        resetCounters();
        if (buttonBeepEnable == true) singleBeep(50);
      }

      /*Toggle audio mode*/
      if (key == 'D') {
        audioMode = !audioMode;
        x = currentTitle.length();
        if (buttonBeepEnable == true) singleBeep(20);
      }
    }
  }

  /*Code for secondary OLED 'subdisplay'*/
  if (oledDisp == 2) {

    /*Run clock page refresh timer*/
    if (masterClock - previousRadioRefresh >= radioInterval || previousRadioRefresh == 0) {
      previousRadioRefresh = masterClock;

      // /*Update radio chip with current settings*/
      // radio.setFrequency(frequency);
      // radio.setStandby(stbyRadio);
      // radio.setStereoNC(snc);
      // radio.setMuted(muted);

      /*Check for stereo signal and process associated options*/
      bool stereo = radio.isStereo();
      if (stereo == true && sncDisable == false) snc = true;
      else if (stereo == true && sncDisable == true) snc = false;

      /*Draw radio on page display*/
      if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled2.clearDisplay();   //Clear display buffer for new frame
        if (stbyRadio == true) {
          oled2.setCursor(48, 0);
          oled2.setTextSize(1);
          oled2.print("Radio Standby");
        } else if (stbyRadio == false && muted == true) {
          oled2.setCursor(50, 0);
          oled2.setTextSize(1);
          oled2.print("Output Muted");
        } else signalLevel(oledDisp, 0, 0);
        oled2.setTextSize(3);
        oled2.setCursor(2, 40);
        oled2.print(frequency, 1);
        oled2.setTextSize(1);
        oled2.print(" MHz");
        if (stbyRadio == false && muted == false) stereoIcon(stereo, oledDisp, 58, 0);
        oled2.display();    //Draw frame on display
      }
    }
  }
}
