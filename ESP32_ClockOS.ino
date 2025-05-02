////////////
//Includes//
////////////
#include <Wire.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <SHT31.h>
#include <TEA5767.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

////////////////
//SHT31D setup//
////////////////
#define SHT31_ADDRESS 0x44    
SHT31 sht;      

////////////////
//DS3231 setup//
////////////////
DS3231 RTC;

/////////////////////////////////
//Keypad and input buffer setup//
/////////////////////////////////
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {4,13,14,27};
byte colPins[COLS] = {26,25,33,32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
char inputBuffer[20];
int bufferIndex = 0;

///////////////////////
//TEA5767 radio setup//
///////////////////////
TEA5767 radio = TEA5767();

//////////////////////////
//I2C OLED display setup//
//////////////////////////
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN1_ADDRESS 0x3C
#define SCREEN2_ADDRESS 0x3D
int rstPIN = 12;
Adafruit_SSD1306 oled1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 oled2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/////////////////////////////////
//Byte bitmaps for OLED display//
/////////////////////////////////
const unsigned char bmp_signal3 [] PROGMEM = {
	0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0
};
const unsigned char bmp_signal2 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0
};
const unsigned char bmp_signal1 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0
};
const unsigned char bmp_signal4 [] PROGMEM = {
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0
};
const unsigned char bmp_signalAntenna [] PROGMEM = {
	0xff, 0xfe, 0xff, 0xfe, 0xf3, 0x9e, 0x7b, 0xbc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 
	0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80
};
const unsigned char bmp_humidity [] PROGMEM = {
	0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x03, 0xc0, 0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x0f, 0xf0, 
	0x0f, 0xf0, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x0f, 0xf0, 0x0f, 0xf0, 0x07, 0xe0, 0x01, 0x80
};
const unsigned char bmp_temp [] PROGMEM = {
	0x01, 0x80, 0x02, 0x40, 0x02, 0x40, 0x02, 0xc0, 0x02, 0x40, 0x02, 0xc0, 0x02, 0x40, 0x02, 0xc0, 
	0x02, 0x40, 0x0e, 0x70, 0x18, 0x18, 0x10, 0x08, 0x10, 0x08, 0x18, 0x18, 0x0e, 0x70, 0x03, 0xc0
};
const unsigned char bmp_stereo [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x00, 0x00, 
	0x38, 0x00, 0x1c, 0x00, 0x92, 0x3c, 0x00, 0x3c, 0x49, 0x49, 0x3e, 0x00, 0x7c, 0x92, 0x24, 0xbf, 
	0xc3, 0xfd, 0x24, 0x24, 0xbf, 0xc3, 0xfd, 0x24, 0x24, 0xbf, 0xc3, 0xfd, 0x24, 0x24, 0xbf, 0xc3, 
	0xfd, 0x24, 0x49, 0x3e, 0x00, 0x7c, 0x92, 0x92, 0x3c, 0x00, 0x3c, 0x49, 0x00, 0x38, 0x00, 0x1c, 
	0x00, 0x00, 0x30, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char bmp_mono [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x38, 0x00, 0x92, 0x3c, 0x00, 0x49, 
	0x3e, 0x00, 0x24, 0xbf, 0xc0, 0x24, 0xbf, 0xc0, 0x24, 0xbf, 0xc0, 0x24, 0xbf, 0xc0, 0x49, 0x3e, 
	0x00, 0x92, 0x3c, 0x00, 0x00, 0x38, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char bmp_timer1 [] PROGMEM = {
	0x00, 0x00, 0x7f, 0xf8, 0x7f, 0xf8, 0x20, 0x10, 0x20, 0x10, 0x38, 0xf0, 0x3f, 0xf0, 0x1f, 0xe0, 
	0x1f, 0xe0, 0x0f, 0xc0, 0x07, 0x80, 0x07, 0x80, 0x07, 0x80, 0x07, 0x80, 0x09, 0x40, 0x10, 0x20, 
	0x10, 0x20, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x7f, 0xf8, 0x7f, 0xf8, 0x00, 0x00
};
const unsigned char bmp_timer2 [] PROGMEM = {
	0x00, 0x00, 0x7f, 0xf8, 0x7f, 0xf8, 0x20, 0x10, 0x20, 0x10, 0x30, 0x70, 0x3c, 0xf0, 0x1d, 0xe0, 
	0x1f, 0xe0, 0x0f, 0xc0, 0x07, 0x80, 0x07, 0x80, 0x07, 0x80, 0x07, 0x80, 0x09, 0x40, 0x10, 0x20, 
	0x10, 0x20, 0x20, 0x10, 0x22, 0x10, 0x23, 0x10, 0x27, 0x90, 0x7f, 0xf8, 0x7f, 0xf8, 0x00, 0x00
};
const unsigned char bmp_timer3 [] PROGMEM = {
	0x00, 0x00, 0x7f, 0xf8, 0x7f, 0xf8, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x30, 0x30, 0x18, 0xe0, 
	0x1f, 0xe0, 0x0f, 0xc0, 0x07, 0x80, 0x07, 0x80, 0x07, 0x80, 0x07, 0x80, 0x09, 0x40, 0x10, 0x20, 
	0x10, 0x20, 0x20, 0x10, 0x23, 0x10, 0x27, 0x90, 0x2f, 0xd0, 0x7f, 0xf8, 0x7f, 0xf8, 0x00, 0x00
};
const unsigned char bmp_timer4 [] PROGMEM = {
	0x00, 0x00, 0x7f, 0xf8, 0x7f, 0xf8, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x30, 0x30, 0x10, 0x20, 
	0x10, 0x60, 0x08, 0xc0, 0x05, 0x80, 0x07, 0x80, 0x07, 0x80, 0x07, 0x80, 0x09, 0x40, 0x10, 0x20, 
	0x10, 0x20, 0x23, 0x10, 0x27, 0x10, 0x2f, 0xd0, 0x3f, 0xd0, 0x7f, 0xf8, 0x7f, 0xf8, 0x00, 0x00
};
const unsigned char bmp_timer5 [] PROGMEM = {
	0x00, 0x00, 0x7f, 0xf8, 0x7f, 0xf8, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x30, 0x30, 0x10, 0x20, 
	0x10, 0x20, 0x08, 0x40, 0x04, 0x80, 0x04, 0x80, 0x04, 0x80, 0x04, 0x80, 0x08, 0x40, 0x10, 0x20, 
	0x16, 0x20, 0x2f, 0x90, 0x3f, 0xd0, 0x3f, 0xf0, 0x3f, 0xf0, 0x7f, 0xf8, 0x7f, 0xf8, 0x00, 0x00
};
const unsigned char bmp_timer6 [] PROGMEM = {
	0x00, 0x00, 0x7f, 0xf8, 0x7f, 0xf8, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x30, 0x30, 0x10, 0x20, 
	0x10, 0x20, 0x08, 0x40, 0x04, 0x80, 0x04, 0x80, 0x04, 0x80, 0x04, 0x80, 0x08, 0x40, 0x10, 0x20, 
	0x10, 0x20, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x20, 0x10, 0x7f, 0xf8, 0x7f, 0xf8, 0x00, 0x00
};

//////////////////////////////////
//Digital/Analog Pin Definitions//
//////////////////////////////////
const int beeper = 16;
const int chime = 17;
const int ldr = 35;

////////////////////////////////////////////////////////////
//Defining variables used in different sections of program//
////////////////////////////////////////////////////////////
unsigned char DoW;
unsigned char hour;
unsigned char minute;
unsigned char second;
unsigned char month;
unsigned char day;
unsigned char year;
int setAMPM;
int setHour;
int setMin;
int setYear;
int setMonth;
int setDay;
int setDoW;
int prevHour;
int timerHour = 0;
int timerMin = 0;
int timerSec = 0;
int timerBMP = 0;
int sigStrength;
int nightModeTime = 15;
unsigned long masterClock;
unsigned long previousTimeRefresh = 0;
unsigned long previousTimerRefresh = 0;
unsigned long previousTimeCount = 0;
unsigned long previousTimerCount = 0;
unsigned long previousTempRefresh = 0;
unsigned long previousRadioRefresh = 0;
unsigned long previousNightModeCount = 0;
const unsigned long timeInterval = 500;
const unsigned long timerInterval = 1000;
const unsigned long tempInterval = 15000;
const unsigned long radioInterval = 2000;
float tempF;
float frequency;
bool h12;
bool AMPM;
bool centuryBit;
bool muted = false;
bool stby = true;
bool snc = false;
bool sncDisable = 0;

/////////////////////////////////////////////////////////////
//Flags used to set displayed pages or functions of program//
/////////////////////////////////////////////////////////////
int lastPage = 1;
int oled2Page;
bool oled2Priority = false;
bool setBit = false;
bool menuShow = false;
bool sysSettingsShow = false;
bool clockShow = true;
bool timerShow = false;
bool timerRun = false;
bool tempShow = false;
bool radioShow = false;
bool nightModeActive = false;

//////////////////
//Settings flags//
//////////////////
bool hourChimeEnable;
bool buttonBeepEnable;
bool infoChimeEnable;
bool nightModeEnable;

/////////////////////////
//Function declarations//
/////////////////////////
/*Display control*/
void oled2PriorityDisp();
void displayBlank(int oledDisp);

/*Menu functions*/
void menu();
void menuOpts(int menuOpt);
void sysSettings();

/*Clock page display and clock setting functions*/
void displayClock(int oledDisp);
void setClock();
void getDoW();
void getHour();
void getMin();
void getAMPM();
void getYear();
void getMonth();
void getDay();
bool confirmSave();

/*Timer page display and timer setting functions*/
void displayTimer(int oledDisp, bool oledOverride = false);
void drawTimerCount(int oledDisp);
void drawTimerBMP(int timerBMP, int oledDisp);
void setTimer();
void getTimerHour();
void getTimerMin();
void getTimerSec();

/*Temp/humidity page display functions*/
void displayTemp(int oledDisp);

/*Radio page display and radio setting functions*/
void displayRadio(int oledDisp);
void setRadio();
void setStandby(bool stby);
void setStereoNC(bool snc);
void setMuted(bool muted);
void signalLevel(int oledDisp);

/*Audible alert generation functions*/
void singleBeep(int beepDur);
void doubleBeep(int beepDur);
void hourChime();
void setChime();
void successChime();
void errorChime();

/*Master timekeeping functions*/
void runMasterClock();
void runTime();
void resetCounters();
void nightModeTimer();
void nightModeActivity();
void runTimer();
void resetTimer(int option);


////////////////////////////
//Initialization and setup//
////////////////////////////
void setup() {
	pinMode(beeper, OUTPUT);
  pinMode(chime, OUTPUT);
	pinMode(rstPIN, OUTPUT);
  digitalWrite(rstPIN, LOW);
  delay(500);
  digitalWrite(rstPIN, HIGH);
  Wire.begin();
  Wire.setClock(100000);
	delay(1000);
	EEPROM.begin(1000);
  sht.begin();
  radio.setStandby(stby);
  radio.setMuted(muted);
	oled1.begin(SSD1306_SWITCHCAPVCC, SCREEN1_ADDRESS);
  oled1.clearDisplay();
  oled1.setTextColor(SSD1306_WHITE);
  if (nightModeActive == false) oled1.display();
	oled2.begin(SSD1306_SWITCHCAPVCC, SCREEN2_ADDRESS);
  oled2.clearDisplay();
  oled2.setTextColor(SSD1306_WHITE);
  if (nightModeActive == false) oled2.display();
	prevHour = RTC.getHour(h12, AMPM);
	sht.requestData();
	frequency = EEPROM.readFloat(0x00), 2;
	hourChimeEnable = EEPROM.readBool(0xA0);
	buttonBeepEnable = EEPROM.readBool(0xA1);
	infoChimeEnable = EEPROM.readBool(0xA2);
	nightModeEnable = EEPROM.readBool(0xA3);
}

/////////////////////
//Main program loop//
/////////////////////
void loop() {

	/*Timekeeping for other functions*/
	runMasterClock();
	runTime();
	runTimer();
	if (nightModeEnable == true) nightModeTimer();

	/*Dim primary display in low light*/
	int lightLevel = analogRead(ldr);
	if (lightLevel < 1024) {
		oled1.dim(true);
	}	else if (lightLevel > 1280) {
		oled1.dim(false);
	}
	
	/*Call priority display page on subscreen*/
	if (oled2Priority == true) oled2PriorityDisp();
	
	/*Call menu display page on main screen and update page on subscreen*/
	if (menuShow == true) {
		if (timerRun == false && stby == true && oled2Priority == false) {
			displayClock(2);
		} else if (timerRun == true && oled2Priority == false) {
    	displayTimer(2);
  	} else if (timerRun == false && stby == false && oled2Priority == false) {
     	displayRadio(2);
   	}
		menu();
	}
	
	/*Call settings display page on main screen and update page on subscreen*/
	if (sysSettingsShow == true) {
		if (timerRun == false && stby == true && oled2Priority == false) {
			displayClock(2);
		} else if (timerRun == true && oled2Priority == false) {
   		displayTimer(2);
   	} else if (timerRun == false && stby == false && oled2Priority == false) {
     	displayRadio(2);
   	}
		sysSettings();
		}
	
	/*Call clock display page on main screen and update page on subscreen*/
	if (clockShow == true) {
		if (timerRun == false && stby == true && oled2Priority == false) {
			displayTemp(2);
		} else if (timerRun == true && oled2Priority == false) {
    	displayTimer(2);
  	} else if (timerRun == false && stby == false && oled2Priority == false) {
   		displayRadio(2);
   	}
		displayClock(1);
	}
	
	/*Call timer display page on main screen and update page on subscreen*/
	if (timerShow == true) {
		if (stby == true && oled2Priority == false) {
			displayClock(2);
		} else if (stby == false && oled2Priority == false) {
    	displayRadio(2);
  	}
		displayTimer(1);
	}
	
	/*Call temp display page on main screen and update page on subscreen*/
	if (tempShow == true) {
		if (timerRun == false && stby == true && oled2Priority == false) {
			displayClock(2);
		} else if (timerRun == true && oled2Priority == false) {
     	displayTimer(2, false);
   	} else if (timerRun == false && stby == false && oled2Priority == false) {
     	displayRadio(2);
   	}
		displayTemp(1);
	}
	
	/*Call radio display page on main screen and update page on subscreen*/
	if (radioShow == true) {
		if (timerRun == false && oled2Priority == false) {
			displayClock(2);
		} else if (timerRun == true && oled2Priority == false) {
   		displayTimer(2);
   	}
		displayRadio(1);
	}	
}

/////////////////////////////////////////////////
//Displays selected priority page on subdisplay//
/////////////////////////////////////////////////
void oled2PriorityDisp() {
	if (oled2Page == 1) {
		displayClock(2);
	}
	if (oled2Page == 2) {
		displayTimer(2);
	}
	if (oled2Page == 3) {
		displayTemp(2);
	}
	if (oled2Page == 4) {
		displayRadio(2);
	}
}

/////////////////////////////////
//Function for display blanking//
/////////////////////////////////
void displayBlank(int oledDisp) {
  if (oledDisp == 1) {
    oled1.clearDisplay();
    oled1.display();
  }
  if (oledDisp == 2) {
    oled2.clearDisplay();
    oled2.display();
  }
}
