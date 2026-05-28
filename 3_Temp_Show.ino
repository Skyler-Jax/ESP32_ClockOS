///////////////////////////////////////////
//Function for Temp/Humidity display page//
///////////////////////////////////////////
void displayTemp(int oledDisp) {

  /*Code for primary OLED display*/
  if (oledDisp == 1) {

    /*Run clock page refresh timer if SHT31 is ready*/
    if (sht.dataReady()) {
      if (masterClock - previousTempRefresh >= tempInterval || previousTempRefresh == 0) {
        previousTempRefresh = masterClock;

        /*Request and read current environmental data from sensor*/
        sht.requestData();
        sht.readData(true);
        tempF = (sht.getTemperature() * 1.8) + 31.4;    //Convert Celsius value from sensor to Farenheit

        /*Draw temp on page display*/
        if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
          oled1.clearDisplay();   //Clear display buffer for new frame
          oled1.drawBitmap(4, 7, bmp_temp, 16, 16, WHITE);
          oled1.drawLine(0, 31, 127, 31, SSD1306_WHITE);
          oled1.drawLine(0, 32, 127, 32, SSD1306_WHITE);
          oled1.drawBitmap(4, 42, bmp_humidity, 16, 16, WHITE);
          oled1.setTextSize(3);
          oled1.setCursor(27,4);
          oled1.print(tempF, 1);
          oled1.setTextSize(2);
          oled1.print(" F");
          oled1.setCursor(27, 38);
          oled1.setTextSize(3);
          oled1.print(sht.getHumidity(), 1);
          oled1.setTextSize(2);
          oled1.print(" %");
          oled1.display();    //Draw frame on display
        }
      }
    }

    /*Read keypad key press and process selection*/
    char key = keypad.getKey();
    if (key != NO_KEY){
      delay(50);    //Debounce timer
      if (nightModeEnable == true) nightModeActivity();   //Wake from night mode

      /*Return to Main Menu*/
      if (key == '#') {
        tempShow = false;
        menuShow = true;
        resetCounters();
        if (buttonBeepEnable == true) singleBeep(50);
      }

      /*Toggle current page for subdisplay priority*/
      if (key == 'D') {
        if (oled2Priority == true && oled2Page == 3) oled2Priority = 0;
        else {
          oled2Priority = 3;
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

    /*Run clock page refresh timer if SHT31 is ready*/
    if (sht.dataReady()) {
      if (masterClock - previousTempRefresh >= tempInterval || previousTempRefresh == 0) {
        previousTempRefresh = masterClock;

        /*Request and read current environmental data from sensor*/
        sht.requestData();
        sht.readData(false);
        tempF = (sht.getTemperature() * 1.8) + 31.4;    //Convert Celsius value from sensor to Farenheit

        /*Draw temp on page display*/
        if (nightModeActive == false) {   //Only draw when Night Mode blanking inactive
          oled2.clearDisplay();   //Clear display buffer for new frame
          oled2.drawBitmap(4, 7, bmp_temp, 16, 16, WHITE);
          oled2.drawLine(0, 31, 127, 31, SSD1306_WHITE);
          oled2.drawLine(0, 32, 127, 32, SSD1306_WHITE);
          oled2.drawBitmap(4, 42, bmp_humidity, 16, 16, WHITE);
          oled2.setTextSize(3);
          oled2.setCursor(27,4);
          oled2.print(tempF, 1);
          oled2.setTextSize(2);
          oled2.print(" F");
          oled2.setCursor(27, 38);
          oled2.setTextSize(3);
          oled2.print(sht.getHumidity(), 1);
          oled2.setTextSize(2);
          oled2.print(" %");
          oled2.display();    //Draw frame on display
        }
      }
    }
  }
}
