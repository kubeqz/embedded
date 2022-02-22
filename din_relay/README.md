DIN relay module

Microcontroller: ATtiny2313
External connections:
 * PD5 -> relay control
 * PD3 -> activity led
 * PD3 -> relay on/off LED

How it works ?
ATtiny is configured to receive commands over UART, baud 19200.
Frame is 8-byte long, with CRC-8 in the last byte.
CMD[0] -> address, should be 0x10
CMD[1] -> command: 0 -> off, 1 -> on, 2 -> on with timeout
CMD[2], CMD[3] -> determines timeout value in seconds (big endian)
CMD[7] -> CRC-8 of all previous bytes

Activity LED blinks once per two seconds.
