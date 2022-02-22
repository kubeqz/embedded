#include "led.h"
#include "timer.h"

#include <avr/io.h>
#include <stdint.h>

constexpr uint8_t RedLedBitMask = 1 << PD3;

void LED_init()
{
    DDRD |= RedLedBitMask;
}

void LED_process()
{
    if (timer_10ms[0] > 200U)
    {
        if (timer_10ms[0] > 205)
        {
            PORTD &= ~RedLedBitMask;
            timer_10ms[0] = 0;
        }
        else
        {
            PORTD |= RedLedBitMask;
        }
    }
}