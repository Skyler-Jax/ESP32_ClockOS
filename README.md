# ESP32_ClockOS
ClockOS for ESP32 is a simple system to run clock radio functions on an ESP32 with accompanying circuits. It features support for dual displays and a full 16 key keypad for direct user input. 

This project is still in development, and will be updated with new features and code optimizations as I work on becoming less of a n00b.

Ancillary circuits required:
1. DS3231 RTC
2. SHT31 temp/humidity sensor
3. TEA5767 FM radio

Additional components required:
1. 1.5" 128x64 SSD1309 OLED display
2. .9" 128x64 SSD1306 OLED display
3. digital I/O-to-I2C keypad converter
4. generic 4x4 matrix keypad
5. active buzzer for alarm and button beeps
6. passive buzzer for simple chimes
7. LDR for display dimming and Night Mode activation

While my end-game goal is to write all of my own libraries for the finished product, this project currently utilizes the following additional libraries:
1. https://github.com/NorthernWidget/DS3231
2. https://github.com/RobTillaart/SHT31
3. https://github.com/simonmonk/arduino_TEA5767
4. https://github.com/adafruit/Adafruit_SSD1306
5. https://github.com/adafruit/Adafruit-GFX-Library
6. https://github.com/RobTillaart/I2CKeyPad
