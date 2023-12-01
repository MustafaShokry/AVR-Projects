#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "BIT_MATH.h"

#define UNITS_START 0
#define TENS_START 0
#define DIRECTION_START true

#define PIN_RESET PC1
#define PIN_TOGGLE_DIRECTION PC2
#define PIN_ON_OFF PC0

#define DELAY_MS 10

void handleButtonPress(uint8_t pin, bool *countingUp, int *units, int *tens) {
    if (GET_BIT(PINC, pin) == 0) {
        while (GET_BIT(PINC, pin) == 0) { /* Wait for button release */ }
        _delay_ms(DELAY_MS);

        if (pin == PIN_RESET) {
            *units = UNITS_START;
            *tens = TENS_START;
            *countingUp = DIRECTION_START;
        } else if (pin == PIN_TOGGLE_DIRECTION) {
            *countingUp = !(*countingUp);
        }
    }
}

int main(void) {
    int units = UNITS_START, tens = TENS_START;
    bool countingUp = DIRECTION_START;

    uint8_t common_anode_segment_values[10] =
        {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

    DDRA = 0xFF;
    DDRB = 0xFF;
    DDRC = 0;

    SET_BIT(PORTC, PIN_RESET);
    SET_BIT(PORTC, PIN_TOGGLE_DIRECTION);
    SET_BIT(PORTC, PIN_ON_OFF);

    while (1) {
        if (GET_BIT(PINC, PIN_ON_OFF) == 0) {
            handleButtonPress(PIN_RESET, &countingUp, &units, &tens);
            handleButtonPress(PIN_TOGGLE_DIRECTION, &countingUp, &units, &tens);

            PORTB = ~common_anode_segment_values[units];
            PORTA = ~common_anode_segment_values[tens];
            _delay_ms(300);

            if (countingUp) {
                units++;
            } else {
                units--;
            }

            if (units == 9 && tens == 9) {
                countingUp = false;
            } else if (units == 0 && tens == 0) {
                countingUp = true;
            }

            if (units == 10 && countingUp) {
                units = 0;
                tens++;
            } else if (units == -1 && !countingUp) {
                units = 9;
                tens--;
            }
        } else {
            PORTB = 0;
            PORTA = 0;
        }
    }

    return 0;
}
