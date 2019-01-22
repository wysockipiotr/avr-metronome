<h1 align="center">AVR Metronome</h1>
<h5 align="center">Metronome with speaker and remote vibration module</h5>

#### Modules
<table>
<tr>
  <td style="text-align: center">
    <img src="https://github.com/wysockipiotr/avr-metronome/blob/master/doc/img/main.jpg" height="320px" />
  </td>
  <td style="text-align: center">
    <img src="https://github.com/wysockipiotr/avr-metronome/blob/master/doc/img/remote.jpg" height="320px" />
  </td>
</tr>
</table>

#### Built with
- ATmega 168A-PU
- Qduino Mini
- MCP4161
- TPA2005D1
- HM-10 BLE

#### Features
- [x] Sound mode with volume control
- [x] Rotary encoder (with push button) user interface
- [x] BPM and basic signatures control
- [x] Tap tempo (button)
- [x] PWM LCD backlight (dimming after a period of inactivity)
- [x] Storing metronome settings in the internal EEPROM
- [x] Custom character display
- [x] Remote vibration module (BLE)
- [x] Default idle state for remote module, receiving disable signal
- [x] MicroUSB charging
- [x] Adjustable sound pitch

#### Project structure
```c
+-- 
   +-- README.md
   +-- .gitignore
   +-- doc
      +-- eagle
         +-- main
         +-- remote
   +-- metronome          // ATmega metronome main module directory
      +-- Makefile        // compiler and programmer config
      +-- main.c          // entry point
      +-- setup.h         // device main settings
      +-- util.h          // helper macros
      +-- global.h        // extern declarations
      +-- lcd.h           // LCD 2x16 interfacing in 4-bit mode
      +-- lcd.c
      +-- eeprom.h
      +-- eeprom.c
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
