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
    a2dp_sink.set_auto_reconnect(startBT, 1000);
    a2dp_sink.start("ClockOS");
  }
	minX = -12 * currentTitle.length();

  /*Code for primary OLED display*/
  if (oledDisp == 1) {

    /*Run clock page refresh timer*/
    if (masterClock - previousRadioRefresh >= btInterval || previousRadioRefresh == 0) {
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
          oled1.setCursor(18, 1);
          if (a2dp_sink.get_peer_name() != "unknown") oled1.print(a2dp_sink.get_peer_name());
          oled1.setCursor(18, 11);
          oled1.print(playStatus);
          if (playStatus == "Playing") {
            if (currentTitle.length() <= 10) oled1.setCursor(0, 24);
            else oled1.setCursor(x, 24);
            oled1.setTextSize(2);
            oled1.print(currentTitle);
            if (currentArtist.length() <= 22) oled1.setCursor(0, 44);
            else oled1.setCursor(x, 44);
            oled1.setTextSize(1);
            oled1.print(currentArtist);
            if (currentAlbum.length() <= 22) oled1.setCursor(0, 54);
            else oled1.setCursor(x, 54);
            oled1.println(currentAlbum);
          }
        }
        oled1.display();    //Draw frame on display
        x = x - 2;
        if (x < minX) x = 128;
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

      /*Playback Controls*/
      if (key == '1') {
        a2dp_sink.previous();
        x = currentTitle.length();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '2') {
        if (playStatus == "Playing") a2dp_sink.pause();
        else a2dp_sink.play();
        x = currentTitle.length();
        if (buttonBeepEnable == true) singleBeep(20);
      }
      if (key == '3') {
        a2dp_sink.next();
        x = currentTitle.length();
        if (buttonBeepEnable == true) singleBeep(20);
      }

      /*Toggle Bluetooth standby mode*/
      if (key == 'A') {
        stbyBT = !stbyBT;
        if (buttonBeepEnable == true) singleBeep(20);
      }

      /*Toggle audio mute*/
      if (key == 'B') {
        muted = !muted;
        if (buttonBeepEnable == true) singleBeep(20);
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


  /*Code for secondary OLED display*/
  if (oledDisp == 2) {

    /*Run clock page refresh timer*/
    if (masterClock - previousRadioRefresh >= btInterval || previousRadioRefresh == 0) {
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
        oled2.setTextSize(1);
        if (stbyBT == false) {
          oled2.setCursor(18, 1);
          if (a2dp_sink.get_peer_name() == "unknown") oled2.print("Disconnected");
          else oled2.print(a2dp_sink.get_peer_name());
          oled2.setCursor(18, 11);
          oled2.print(playStatus);
          if (playStatus == "Playing") {
            if (currentTitle.length() <= 10) oled2.setCursor(0, 24);
            else oled2.setCursor(x, 24);
            oled2.setTextSize(2);
            oled2.print(currentTitle);
            if (currentArtist.length() <= 22) oled2.setCursor(0, 44);
            else oled2.setCursor(x, 44);
            oled2.setTextSize(1);
            oled2.print(currentArtist);
            if (currentAlbum.length() <= 22) oled2.setCursor(0, 54);
            else oled2.setCursor(x, 54);
            oled2.println(currentAlbum);
          }
        }
        oled2.display();    //Draw frame on display
        x = x - 2;
        if (x < minX) x = 128;
      }
    }
  }
}