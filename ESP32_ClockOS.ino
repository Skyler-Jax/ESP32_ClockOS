#pragma region includes
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
// #include <Keypad.h>
#include <I2CKeyPad.h>
#include <WiFi.h>
#include <I2CKeyPad.h>
#pragma endregion includes

#pragma region hardware_setup
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
I2CKeyPad keyPad(0x20);
char keymap[19] = "D#0*C987B654A321NF";  //  N = NoKey, F = Fail
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
#define OLED_RESET -1
#define SCREEN1_ADDRESS 0x3C
#define SCREEN2_ADDRESS 0x3D
int rstPinOLED = 12;
Adafruit_SSD1306 oled1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 oled2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#pragma endregion hardware_setup

#pragma region bitmap_bytecodes
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
const unsigned char bmp_usb [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x07, 0x00, 0x00, 0x1f, 0x80, 0x18, 0x27, 0x08, 0x3c,
	0x42, 0x0c, 0x7e, 0x40, 0x0e, 0x7f, 0xff, 0xff, 0x7e, 0x10, 0x0e, 0x3c, 0x10, 0x0c, 0x18, 0x0b,
	0x88, 0x00, 0x07, 0x80, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00
};
const unsigned char bmp_dc [] PROGMEM = {
	0x00, 0x00, 0x00, 0x01, 0xe3, 0x80, 0x01, 0x36, 0x00, 0x01, 0x14, 0x00, 0x01, 0x14, 0x00, 0x01, 
	0x36, 0x00, 0x01, 0xe3, 0x80, 0x00, 0x00, 0x00, 0xf7, 0xbd, 0xef, 0xf7, 0xbd, 0xef, 0x00, 0x00, 
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00
};
const unsigned char bmp_batt0 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x60, 0x00, 0x18, 0x41, 0x83, 0x08, 0x40, 0xc6, 0x0e, 0x40,
	0x6c, 0x0a, 0x40, 0x38, 0x0a, 0x40, 0x38, 0x0a, 0x40, 0x6c, 0x0a, 0x40, 0xc6, 0x0e, 0x41, 0x83,
	0x08, 0x60, 0x00, 0x18, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00
};
const unsigned char bmp_batt1 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x60, 0x00, 0x18, 0x40, 0x00, 0x08, 0x40, 0x00, 0x0e, 0x40,
	0x00, 0x0a, 0x40, 0x00, 0x0a, 0x40, 0x00, 0x0a, 0x40, 0x00, 0x0a, 0x40, 0x00, 0x0e, 0x40, 0x00,
	0x08, 0x60, 0x00, 0x18, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00
};
const unsigned char bmp_batt2 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x60, 0x00, 0x18, 0x40, 0x00, 0x08, 0x46, 0x00, 0x0e, 0x4f,
	0x00, 0x0a, 0x4f, 0x00, 0x0a, 0x4f, 0x00, 0x0a, 0x4f, 0x00, 0x0a, 0x46, 0x00, 0x0e, 0x40, 0x00,
	0x08, 0x60, 0x00, 0x18, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00
};
const unsigned char bmp_batt3 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x60, 0x00, 0x18, 0x40, 0x00, 0x08, 0x47, 0x70, 0x0e, 0x4f,
	0x78, 0x0a, 0x4f, 0x78, 0x0a, 0x4f, 0x78, 0x0a, 0x4f, 0x78, 0x0a, 0x47, 0x70, 0x0e, 0x40, 0x00,
	0x08, 0x60, 0x00, 0x18, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00
};
const unsigned char bmp_batt4 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x60, 0x00, 0x18, 0x40, 0x00, 0x08, 0x47, 0x7b, 0x8e, 0x4f,
	0x7b, 0xca, 0x4f, 0x7b, 0xca, 0x4f, 0x7b, 0xca, 0x4f, 0x7b, 0xca, 0x47, 0x7b, 0x8e, 0x40, 0x00,
	0x08, 0x60, 0x00, 0x18, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00
};
#pragma endregion bitmap_bytecodes

#pragma region definitions_variables
//////////////////////////////////
//Digital/Analog Pin Definitions//
//////////////////////////////////
const int pwrEn = 23;
const int boardLED = 2;
const int beeper = 16;
const int chime = 17;
const int ldr = 35;
const int battery = 34;

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
int battLvl;
int nightModeTime = 6;
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
float tempComp;
float frequency;
bool h12;
bool AMPM;
bool centuryBit;
bool muted = false;
bool stby = true;
bool snc = false;
bool sncDisable = 0;
bool onDC;

/////////////////////////////////////////////////////////////
//Flags used to set displayed pages or functions of program//
/////////////////////////////////////////////////////////////
int lastPage = 1;
int oled2Page = 0;
int oled2Priority = 0;
bool setBit = false;
bool menuShow = false;
bool sysSettingsShow = false;
bool tempSettingsShow = false;
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
bool tempCF;
#pragma endregion definitions_variables

#pragma region function_declarations
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
void tempSettings();
void setTempComp();

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

/*Audible alert generation functions*/
void singleBeep(int beepDur);
void doubleBeep(int beepDur);
void hourChime();
void setChime();
void successChime();
void errorChime();

/*Graphical element generation*/
void dcInputIcon(bool onDC, int oledDisp, int posX, int posY);
void drawTimerBMP(int timerBMP, int oledDisp, int posX, int posY);
void signalLevel(int oledDisp, int posX, int posY);
void stereoIcon(bool stereo, int oledDisp, int posX, int posY);

/*Master timekeeping functions*/
void runMasterClock();
void runTime();
void resetCounters();
void nightModeTimer();
void nightModeActivity();
void runTimer();
void resetTimer(int option);

/*Misc functions*/
void battLevel(int battChk);
char getKeyChar();
#pragma endregion function_declarations

#pragma region setup
////////////////////////////
//Initialization and setup//
////////////////////////////
void setup() {
	Serial.begin(115200);
	pinMode(pwrEn, OUTPUT);
	pinMode(boardLED, OUTPUT);
	pinMode(beeper, OUTPUT);
  pinMode(chime, OUTPUT);
	pinMode(rstPinOLED, OUTPUT);
	digitalWrite(pwrEn, HIGH);
	digitalWrite(boardLED, LOW);
  digitalWrite(rstPinOLED, LOW);
  delay(500);
  digitalWrite(rstPinOLED, HIGH);
  Wire.begin(21,22);
	Wire.setClock(400000);
	delay(1000);
	keyPad.loadKeyMap(keymap);
	EEPROM.begin(1000);
  sht.begin();
  radio.setStandby(stby);
  radio.setMuted(muted);
	oled1.begin(SSD1306_SWITCHCAPVCC, SCREEN1_ADDRESS);
  oled1.clearDisplay();
  oled1.setTextColor(SSD1306_WHITE);
  oled1.display();
	oled2.begin(SSD1306_SWITCHCAPVCC, SCREEN2_ADDRESS);
  oled2.clearDisplay();
  oled2.setTextColor(SSD1306_WHITE);
  oled2.display();
	prevHour = RTC.getHour(h12, AMPM);
	sht.requestData();
	frequency = EEPROM.readFloat(0x00), 2;
	hourChimeEnable = EEPROM.readBool(0xA0);
	infoChimeEnable = EEPROM.readBool(0xA1);
	buttonBeepEnable = EEPROM.readBool(0xA2);
	nightModeEnable = EEPROM.readBool(0xA3);
	tempCF = EEPROM.readBool(0xB0);
	tempComp = EEPROM.readFloat(0XB1);
}
#pragma endregion setup

#pragma region loop
/////////////////////
//Main program loop//
/////////////////////
void loop() {

	/*Timekeeping for other functions
		Related functions in 5_Misc*/
	runMasterClock();
	runTime();
	runTimer();
	if (nightModeEnable == true) nightModeTimer();
	


	/*Detect power source & low battery protection
		Related functions in 5_Misc*/
  int battChg = analogRead(battery);
	battLevel(battChg);
	if (battChg == 0) onDC = false;
	else if (battChg > 0) onDC = true;
	//if (battChg < 740 && battChg != 0) esp_deep_sleep_start(); //Disabled until correct batt levels established

	/*Dim primary display in low light & blank display for night mode*/
	int lightLevel = analogRead(ldr);
	if (lightLevel < 76) {
		oled1.dim(true);
		digitalWrite(boardLED, HIGH);
	}
	else if (lightLevel > 102) {
		oled1.dim(false);
		digitalWrite(boardLED, LOW);
	}
	if (lightLevel < 76 && nightModeTime == 0) {
    nightModeActive = true;
    displayBlank(1);
    displayBlank(2);
  }
	else if (lightLevel > 102 && nightModeActive == true) {
		nightModeActive = false;
		resetCounters();
		oled1.display();
		oled2.display();
	}

	/*Call priority display page on subscreen
		Requires further debugging, disabled for now*/
	//if (oled2Priority != 0) oled2PriorityDisp();

	/*Call menu display page on main screen and update page on subscreen*/
	if (menuShow == true) {
		if (timerRun == false && stby == true && oled2Priority == 0) {
			oled2Page = 1;
			// displayClock(2);
		} else if (timerRun == true && oled2Priority == 0) {
			oled2Page = 2;
    // 	displayTimer(2);
  	} else if (timerRun == false && stby == false && oled2Priority == 0) {
			oled2Page = 4;
    //  	displayRadio(2);
   	}
		menu();
	}

	/*Call settings display page on main screen and update page on subscreen*/
	if (sysSettingsShow == true) {
		if (timerRun == false && stby == true && oled2Priority == 0) {
			oled2Page = 1;
			displayClock(2);
		} else if (timerRun == true && oled2Priority == 0) {
			oled2Page = 2;
   		displayTimer(2);
   	} else if (timerRun == false && stby == false && oled2Priority == 0) {
			oled2Page = 4;
     	displayRadio(2);
   	}
		sysSettings();
	}

	/*Call settings display page on main screen and update page on subscreen*/
	if (tempSettingsShow == true) {
		tempSettings();
		displayTemp(2);
	}

	/*Call clock display page on main screen and update page on subscreen*/
	if (clockShow == true) {
		if (timerRun == false && stby == true && oled2Priority == 0) {
			oled2Page = 3;
			displayTemp(2);
		} else if (timerRun == true && oled2Priority == 0) {
			oled2Page = 2;
    	displayTimer(2);
  	} else if (timerRun == false && stby == false && oled2Priority == 0) {
			oled2Page = 4;
   		displayRadio(2);
   	}
		displayClock(1);
	}

	/*Call timer display page on main screen and update page on subscreen*/
	if (timerShow == true) {
		if (stby == true && oled2Priority == 0) {
			oled2Page = 1;
			displayClock(2);
		} else if (stby == false && oled2Priority == 0) {
			oled2Page = 4;
    	displayRadio(2);
  	}
		displayTimer(1);
	}

	/*Call temp display page on main screen and update page on subscreen*/
	if (tempShow == true) {
		if (timerRun == false && stby == true && oled2Priority == 0) {
			oled2Page = 1;
			displayClock(2);
		} else if (timerRun == true && oled2Priority == 0) {
			oled2Page = 2;
     	displayTimer(2);
   	} else if (timerRun == false && stby == false && oled2Priority == 0) {
			oled2Page = 4;
     	displayRadio(2);
   	}
		displayTemp(1);
	}

	/*Call radio display page on main screen and update page on subscreen*/
	if (radioShow == true) {
		if (timerRun == false && oled2Priority == 0) {
			oled2Page = 1;
			displayClock(2);
		} else if (timerRun == true && oled2Priority == 0) {
			oled2Page = 2;
   		displayTimer(2);
   	}
		displayRadio(1);
	}

#pragma region serial_diag
	/*Diagnostic output to TTY*/
	// Serial.write(27);
	// Serial.print("[2J"); // clear screen
	// Serial.write(27);
	// Serial.print("[H"); // cursor to home
	Serial.print("   Light Level (Analog)-");
	Serial.print(lightLevel);
	Serial.print("   Night Mode Timer-");
	Serial.print(nightModeTime);
	Serial.print("   Night Mode Active-");
	if (nightModeActive == true) Serial.print("Yes");
	if (nightModeActive == false) Serial.print("No ");
	Serial.print("   Battery Level (Analog)-");
	Serial.print(battChg);
	Serial.print("   Battery Level (Calculated)-");
	Serial.print(battLvl);
	Serial.println();
	#pragma endregion serial_diag
}
#pragma endregion loop


/////////////////////////////////////////////////
//Displays selected priority page on subdisplay//
/////////////////////////////////////////////////
void oled2PriorityDisp() {
	if (oled2Priority == 1) {
		oled2Page = 1;
		displayClock(2);
	}
	if (oled2Priority == 2) {
		oled2Page = 2;
		displayTimer(2);
	}
	if (oled2Priority == 3) {
		oled2Page = 3;
		displayTemp(2);
	}
	if (oled2Priority == 4) {
		oled2Page = 4;
		displayRadio(2);
	}
}

////////////////////
//Display blanking//
////////////////////
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
