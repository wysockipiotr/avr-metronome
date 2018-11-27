# avr-metronome

#### Features
- [x] Sound mode with volume control
- [x] Rotary encoder (with push button) user interface
- [x] BPM and basic signatures control
- [x] Tap tempo (button)
- [x] PWM LCD backlight (dimming after a period of inactivity)
- [ ] Storing metronome settings in EEPROM
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
      +-- main.c          // Entry point
      +-- setup.h         // device main settings
      +-- util.h          // helper macros
      +-- global.h        // extern declarations
      +-- lcd.h           // LCD 2x16 interfacing in 4-bit mode
      +-- lcd.c
      +-- metronome.h     // Core logic of the metronome, ISR handlers
      +-- metronome.c
      +-- usart.h         // Serial communication with HM-10 BLE module
      +-- usart.c
      +-- mcp41xx.h       // MCP4161 Digital potentiometer SPI interfacing
      +-- mcp41xx.c
   +-- remote_module      // Qduino (Arduino) remote vibrating module sketch directory
      +-- README.md
      +-- remote_module.ino
```
