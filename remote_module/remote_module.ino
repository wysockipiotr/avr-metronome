#include <SoftwareSerial.h>

#define MOTOR   1
#define LED     0

uint16_t bpm;
uint16_t period;
uint8_t signature;
uint8_t i;

void pulse(uint16_t duration, uint8_t duty_cycle);

void setup() {
    pinMode(MOTOR, OUTPUT);
    pinMode(LED, OUTPUT);
    bpm = 130u;
    period = 60000 / bpm;
    signature = 0x44;
}

void loop() {
    pulse(period, 85);

    for (i = 0u; i < (signature >> 4) - 1; ++i) {
        pulse(period, 45);
    }
}

void pulse(uint16_t duration, uint8_t duty_cycle) {
    uint16_t t_high = duration * duty_cycle / 100u;
    digitalWrite(MOTOR, HIGH);
    digitalWrite(LED, HIGH);
    delay(t_high);
    digitalWrite(MOTOR, LOW);
    digitalWrite(LED, LOW);
    delay(duration - t_high);
}
