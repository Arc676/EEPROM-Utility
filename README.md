# EEPROM Utility

Toolkit for reading from and writing to EEPROM chips using an Arduino. Designed for Arduino Nano and AT27C256 EEPROM.

## Components

### Arduino

The Arduino microcontroller serves as the interface between the computer and the EEPROM chip. It receives commands via the serial connection and either reads from the serial connection and writes to the EEPROM chip or vice versa. Data reading and writing is sequential and starts at address 0.

### Interfacing Library

The interfacing library acts as an abstraction layer for the serial communication protocol.

### EEPROM Programmer

The graphical frontend uses [ImGui](https://github.com/ocornut/imgui) (MIT licensed) and allows the user to view the data on the EEPROM chip as well as modify it.

## License

Project available under GPLv3. Arduino code based on EEPROM programmer by [Ben Eater](https://github.com/beneater/eeprom-programmer) available under MIT.
