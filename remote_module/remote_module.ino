#include <SoftwareSerial.h>

#include "Qduino.h"

#define MOTOR       9   // vibration motor PWM output
#define RX          14  // RX receives from BLE TX
#define TX          15  // TX transmits to BLE RX

uint16_t            bpm;
volatile uint16_t   period;
volatile uint8_t    signature;
uint8_t             i;

int                 k = 0;
uint16_t            dat[3];
uint8_t             signatures[4] = {0x44, 0x34, 0x54, 0x74};

qduino              q;  // initialize the Qduino library
SoftwareSerial      ble(RX, TX);

void pulse(uint16_t duration, uint8_t duty_cycle);

void setup() {
    pinMode(MOTOR, OUTPUT);

    bpm = 130u;
    period = 60000 / bpm;
    signature = 0x44;

    Serial.begin(9600);
    ble.begin(9600);
}

void loop() {
    while (ble.available() > 0) {
        dat[k++] = ble.read();
        if (k == 3) {
            bpm = (dat[0] << 8) | (dat[1] & 0xff);
            period = 60000 / bpm;
            signature = signatures[dat[2] % 4];
            Serial.print(bpm);
            Serial.print(' ');
            Serial.print(signature >> 4);
            Serial.print('/');
            Serial.print(signature & 0x0f);
            Serial.print('\n');
        }
        k %= 3;
    }

    pulse(period, 85);

    for (i = 0u; i < (signature >> 4) - 1; ++i) {
        pulse(period, 45);
    }
}

void pulse(uint16_t duration, uint8_t duty_cycle) {
    uint16_t t_high = duration * duty_cycle / 100u;

    analogWrite(MOTOR, (duty_cycle > 80) ? 250 : 200);
    q.setRGB((duty_cycle > 80) ? RED : GREEN);
    delay(t_high);

    analogWrite(MOTOR, 0);
    q.ledOff();
    delay(duration - t_high);
}
