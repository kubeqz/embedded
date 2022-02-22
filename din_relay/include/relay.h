#ifndef RELAY_H
#define RELAY_H

#include <stdint.h>

void RELAY_init();
void RELAY_set(bool value);
void RELAY_set_with_timeout(uint16_t timeoutSeconds);
void RELAY_process();

#endif // RELAY_H