#include "cmd.h"
#include "crc.h"
#include "uart.h"
#include "relay.h"
#include "timer.h"

#include <avr/io.h>
#include <stdint.h>
#include <string.h>

constexpr uint8_t Address = 0x10;

static bool CMD_check(uint8_t *cmd, uint8_t len)
{
    return (cmd[0] == Address) && (CRC_calc(cmd, len) == 0);
}

void CMD_process()
{
    static uint8_t cmd[8] = {};
    static uint8_t index = 0;

    uint8_t byte = 0;
    if (UART_getc(&byte))
    {
        if (index < sizeof(cmd))
        {
            cmd[index++] = byte;
        }

        // reset timeout
        timer_10ms[1] = 0;
    }
    else if ((index != 0) && (timer_10ms[1] > 5))
    {
        // command received, check
        if (CMD_check(cmd, index))
        {
            if (cmd[1] == 2) {
                uint16_t timeout = static_cast<uint16_t>(cmd[2]) << 8 | cmd[3];
                RELAY_set_with_timeout(timeout);
            } if (cmd[1] == 1) {
                RELAY_set(true);
            } else if (cmd[1] == 0) {
                RELAY_set(false);
            }
        }

        memset(cmd, 0, sizeof(cmd));
        index = 0;
    }
}
