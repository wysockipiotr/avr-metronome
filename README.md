# avr-metronome

#### Features
- [x] Sound mode with volume control
- [x] Rotary encoder (with push button) user interface
- [x] BPM and basic signatures control
- [x] Tap tempo (button)
- [x] PWM LCD backlight (dimming after a period of inactivity)
- [ ] Storing metronome settings in the internal EEPROM
- [ ] Remote vibration module (BLE)
- [ ] Custom character display

#### Project structure
```c
+-- 
   +-- README.md
   +-- .gitignore
   +-- doc
   +-- metronome          // ATmega metronome main module directory
      +-- Makefile        // compiler and programmer config
      +-- main.c          // entry point
      +-- setup.h         // device main settings
      +-- util.h          // helper macros
      +-- global.h        // extern declarations
      +-- lcd.h           // LCD 2x16 interfacing in 4-bit mode
      +-- lcd.c
      +-- metronome.h     // core logic of the metronome, ISR handlers
      +-- metronome.c
      +-- usart.h         // serial communication with HM-10 BLE module
      +-- usart.c
      +-- mcp41xx.h       // MCP4161 Digital potentiometer SPI interfacing
      +-- mcp41xx.c
   +-- remote_module      // Qduino (Arduino) remote vibrating module sketch directory
      +-- README.md
      +-- remote_module.ino
```


#### Setup
##### Prerequisites
- make
- avrdude
- avr-gcc
##### Makefile
Override parameters in Makefile to suit your hardware configuration. Here, Arduino Uno is used as an in-system programmer.
```diff
- MCU = atmega168
+ MCU = <your_unit>

- PROGRAMMER_TYPE = avrisp
- PROGRAMMER_ARGS = -b 19200 -P COM4
+ PROGRAMMER_TYPE = <your_programmer_type>
+ PROGRAMMER_ARGS = -b <baud_rate> -P <device_name>
```
##### Deployment
Use `make flash` to compile and save program in flash memory (using `avrdude`). Use `make size` to find out about the size of the program.

#### Main module
##### Schematic with connectors
![metronome-schematic](https://github.com/96wysocki/avr-metronome/blob/master/doc/metronome-main.png)
