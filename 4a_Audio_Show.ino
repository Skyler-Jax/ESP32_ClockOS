///////////////////////////////////
//Function for audio display page//
///////////////////////////////////
void displayAudio(int oledDisp) {

  /*Code for primary OLED display*/
  if (oledDisp == 1) {
    if (audioMode == true) modeRadio(1);
    if (audioMode == false) {
      modeBT(1);
      oled1.clearDisplay();
    } 
  }

  /*Code for primary OLED display*/
  if (oledDisp == 2) {
    if (audioMode == true) modeRadio(2);
    if (audioMode == false) modeBT(2);
  }
}
