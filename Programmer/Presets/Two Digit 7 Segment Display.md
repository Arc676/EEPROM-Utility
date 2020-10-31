# Two Digit 7 Segment Displays

## SH3261AS

The SH3261AS is a common cathode, two digit 7 segment display.

The preset data maps binary numbers as addresses to the hexadecimal representation of signed and unsigned 8 bit numbers. For signed numbers, two's complement is used for the representation. Note that there is no space on the display for a minus sign, as 8 bits of signed integers still occupy up to 2 digits. Making the negative sign visible to the user is left as a responsibility of the circuit builder.

### Data

Addresses 0 - 255: outputs for the first digit of the numbers from 0 to 255
Addresses 256 - 511: outputs for the second digit of the numbers from 0 to 255
Addresses 512 - 767: outputs for the first digit of the numbers from -128 to 127
Addresses 768 - 1024: outputs for the second digit of the numbers from -128 to 127

In other words: the least significant 8 bits of the address are the binary representation of the number to display. When the 9th bit is low, the first digit of the number is displayed. When it's high, the second digit is displayed. When the 10th address bit is high, the addresses are interpreted as the two's complement representation of the number as opposed to the unsigned representation.
