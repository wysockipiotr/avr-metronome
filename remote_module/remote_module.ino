#define NO_DEBUG

#include <SoftwareSerial.h>

#include "Qduino.h"

#define MOTOR 9  // vibration motor PWM output
#define RX 14    // RX receives from BLE TX
#define TX 15    // TX transmits to BLE RX

#define ACCENT true
#define ACCENT_PWM 250u
#define ACCENT_DUTY 85u
#define ACCENT_COLOR GREEN
#define NO_ACCENT false
#define NO_ACCENT_PWM 200u
#define NO_ACCENT_DUTY 45u
#define NO_ACCENT_COLOR WHITE
#define IDLE_COLOR RED

// tempo in beats per minute
uint16_t bpm;

// metronome period
volatile uint16_t period;

// active time signature
volatile uint8_t signature;

// vibration enabled
volatile bool enabled = false;

// loop iterator
uint8_t i;

// ble buffer iterator
int k = 0;

// ble data buffer
uint16_t ble_buffer[3];

// time signatures
const uint8_t signatures[4] = {0x44, 0x34, 0x54, 0x74};

// Qduino Mini library
qduino q;

// Bluetooth Low Energy serial
SoftwareSerial ble(RX, TX);

void pulse(uint16_t duration, bool accent);

void read_from_ble();

void setup() {
    pinMode(MOTOR, OUTPUT);

    bpm = 130u;
    period = 60000 / bpm;
    signature = 0x44;

#ifndef NO_DEBUG
    Serial.begin(9600);
#endif
    ble.begin(9600);

    // power on indication
    q.setRGB(IDLE_COLOR);
}

void loop() {
    read_from_ble();

    if (enabled) {
        pulse(period, ACCENT);

        for (i = 0u; i < (signature >> 4) - 1; ++i) {
            pulse(period, NO_ACCENT);
        }
    }
}

void read_from_ble() {
    while (ble.available() > 0) {
        ble_buffer[k++] = ble.read();

        if (k == 3) {
            if (ble_buffer[0] | ble_buffer[1] | ble_buffer[2]) {
                bpm = (ble_buffer[0] << 8) | (ble_buffer[1] & 0xff);
                period = 60000 / bpm;
                signature = signatures[ble_buffer[2] % 4];
                enabled = true;
            } else {
                enabled = false;
                q.setRGB(IDLE_COLOR);
            }

#ifndef NO_DEBUG
            Serial.print(bpm);
            Serial.print(' ');
            Serial.print(signature >> 4);
            Serial.print('/');
            Serial.print(signature & 0x0f);
            Serial.print('\n');
#endif
        }
        k %= 3;
    }
}

void pulse(uint16_t duration, bool accent) {
    uint8_t duty = accent ? ACCENT_DUTY : NO_ACCENT_DUTY;
    uint16_t t_high = duration * duty / 100u;

    analogWrite(MOTOR, accent ? ACCENT_PWM : NO_ACCENT_PWM);
    q.setRGB(accent ? ACCENT_COLOR : NO_ACCENT_COLOR);
    delay(t_high);

    analogWrite(MOTOR, 0);
    q.ledOff();
    delay(duration - t_high);
}
