#include "relay.h"
#include "timer.h"

#include <avr/io.h>

constexpr uint8_t RelayStatusLedMask = 1U << PD4;
constexpr uint8_t RelayControlMask = 1U << PD5;

static uint16_t scheduledTimeoutS = 0;

void RELAY_init()
{
    DDRD |= RelayStatusLedMask | RelayControlMask;
}

void RELAY_set(bool value)
{
    // in case of previous set with timeout cancel it
    scheduledTimeoutS = 0;

    if (value) {
        PORTD |= RelayStatusLedMask | RelayControlMask;
    } else {
        PORTD &= ~(RelayStatusLedMask | RelayControlMask);
    }
}

void RELAY_set_with_timeout(uint16_t timeoutSeconds)
{
    PORTD |= RelayStatusLedMask | RelayControlMask;
    scheduledTimeoutS = timeoutSeconds;
}

void RELAY_process()
{
    if (scheduledTimeoutS != 0)
    {
        if (timer_10ms[2] >= 100) {
            timer_10ms[2] = 0;
            --scheduledTimeoutS;
        }

        if (scheduledTimeoutS == 0) {
            PORTD &= ~(RelayStatusLedMask | RelayControlMask);
        }
    }
}