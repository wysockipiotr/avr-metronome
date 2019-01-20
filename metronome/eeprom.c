#include "eeprom.h"
#include "global.h"
#include "setup.h"

#include <avr/eeprom.h>

void eeprom_store_settings(void) {
    eeprom_update_word(EEPROM_STORE_BPM, bpm);
    eeprom_update_byte(EEPROM_STORE_SIG, signature);
    eeprom_update_byte(EEPROM_STORE_VOL, volume);
    eeprom_update_byte(EEPROM_STORE_MODE, mode);
    eeprom_update_byte(EEPROM_STORE_PITCH, pitch_index);
}

void eeprom_load_settings(void) {
    // load values from eeprom
    bpm = eeprom_read_word(EEPROM_STORE_BPM);
    signature = eeprom_read_byte(EEPROM_STORE_SIG);
    volume = eeprom_read_byte(EEPROM_STORE_VOL);
    mode = eeprom_read_byte(EEPROM_STORE_MODE);
    pitch_index = eeprom_read_byte(EEPROM_STORE_PITCH);

    // normalize values
    if (!within(bpm, MIN_BPM, MAX_BPM)) {
        bpm = DEFAULT_BPM;
    }
    
    if (!within(signature, MIN_SIGNATURE, MAX_SIGNATURE)) {
        signature = DEFAULT_SIGNATURE;
    }

    if (!within(volume, MIN_VOLUME, MAX_VOLUME)) {
        volume = DEFAULT_VOL;
    }

    if (mode >= NUMBER_OF_MODES) {
        mode = DEFAULT_MODE;
    }

    if (!within(pitch_index, MIN_PITCH_INDEX, MAX_PITCH_INDEX)) {
        pitch_index = DEFAULT_PITCH_INDEX;
    }

}