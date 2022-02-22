#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

extern volatile uint16_t timer_10ms[3];

void TIMER_init();
uint32_t TIMER_get();
void TIMER_clear();

#endif // TIMER_H