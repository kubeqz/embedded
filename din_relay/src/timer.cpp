#include "timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>

static volatile uint32_t timer_ms = 0;
volatile uint16_t timer_10ms[3] = {};

void TIMER_init()
{
    // CTC mode, count up to OCR0A
    TCCR0A = 1 << WGM01;
    // divide / 1024
    TCCR0B = (1 << CS00) | (1 << CS02);
    // interrupt every 10ms
    OCR0A = 78;
    // enable compare interrupt
    TIMSK = 1 << OCIE0A;
}

uint32_t TIMER_get()
{
    cli();
    uint32_t val = timer_ms;
    sei();

    return val;
}

void TIMER_clear()
{
    cli();
    timer_ms = 0;
    sei();
}

extern "C"
{

ISR(TIMER0_COMPA_vect)
{
    timer_ms += 10;

    timer_10ms[0]++;
    timer_10ms[1]++;
    timer_10ms[2]++;
}

} // extern "C"
