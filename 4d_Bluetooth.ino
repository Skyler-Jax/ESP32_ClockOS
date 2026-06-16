///////////////////////////////
//Function for Bluetooth mode//
///////////////////////////////
void modeBT(int oledDisp) {
  if (stbyBT == true && startBT == true) {
    startBT = false;
    a2dp_sink.set_connected(startBT);
    a2dp_sink.stop();
  }
  if (stbyBT == false && startBT == false) {
    startBT = true;
    // a2dp_sink.set_connected(startBT);
    a2dp_sink.set_auto_reconnect(startBT, 1000);
    a2dp_sink.start("ClockOS");
  }


  /*Code for primary OLED display*/
  if (oledDisp == 1) {

    /*Run clock page refresh timer*/
    if (masterClock - previousRadioRefresh >= radioInterval || previousRadioRefresh == 0) {
      previousRadioRefresh = masterClock;

      /*Draw BT audio on page display*/
      if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled1.clearDisplay();   //Clear display buffer for new frame
        if (stbyBT == true) {
          oled1.setCursor(51, 0);
          oled1.setTextSize(1);
          oled1.print("BT Standby");
        } else if (stbyBT == false && muted == true) {
          oled1.setCursor(50, 0);
          oled1.setTextSize(1);
          oled1.print("Output Muted");
        }
        audioModeIcon(oledDisp, 0, 0);
        oled1.setTextSize(1);
        if (stbyBT == false) {
          oled1.setCursor(17, 5);
          oled1.print(a2dp_sink.get_peer_name());
          oled1.setCursor(0, 20);
          getMetadata(1);
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

      /*Toggle radio chip standby mode*/
      if (key == 'A') {
        stbyBT = !stbyBT;
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

      /*Toggle audio mode*/
      if (key == 'D') {
        audioMode = !audioMode;      
        if (buttonBeepEnable == true) singleBeep(20);
      }

      /*Return to Main Menu*/
      if (key == '#') {
        audioShow = false;
        menuShow = true;
        resetCounters();
        if (buttonBeepEnable == true) singleBeep(50);
      }
    }
  }

  /*Code for secondary OLED 'subdisplay'*/
  if (oledDisp == 2) {

    /*Run clock page refresh timer*/
    if (masterClock - previousRadioRefresh >= radioInterval || previousRadioRefresh == 0) {
      previousRadioRefresh = masterClock;

      /*Draw BT audio on page display*/
      if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
        oled2.clearDisplay();   //Clear display buffer for new frame
        if (stbyBT == true) {
          oled2.setCursor(51, 0);
          oled2.setTextSize(1);
          oled2.print("BT Standby");
        } else if (stbyBT == false && muted == true) {
          oled2.setCursor(50, 0);
          oled2.setTextSize(1);
          oled2.print("Output Muted");
        }
        audioModeIcon(oledDisp, 0, 0);
        // oled1.setTextSize(3);
        // oled1.setCursor(2, 40);
        // oled1.print(frequency, 1);
        // oled1.setTextSize(1);
        // oled1.print(" MHz");
        // if (stby == false && muted == false) stereoIcon(stereo, oledDisp, 58, 0);
        oled2.display();    //Draw frame on display
      }
    }
  }
}
