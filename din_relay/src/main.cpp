/**
 * @author Jakub Zbydniewski (kubeqz@gmail.com)
 * @brief UART-controlled relay firmware, running on ATTiny2313 @4MHz (internal oscillator)
 * 
 * License: MIT
 */

#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "cmd.h"
#include "led.h"
#include "relay.h"
#include "timer.h"
#include "uart.h"

int main()
{
    LED_init();
    UART_init();
    RELAY_init();
    TIMER_init();

    wdt_reset();
    wdt_enable(WDTO_15MS);

    sei();

    while (true)
    {
        LED_process();
        CMD_process();
        RELAY_process();

        wdt_reset();
    }
}
