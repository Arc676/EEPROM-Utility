# EEPROM Utility

Toolkit for reading from and writing to EEPROM chips using an Arduino. Designed for Arduino Nano and the AT27C256R EPROM and AT28C64B EEPROM microchips.

## Components

### Arduino

The Arduino microcontroller serves as the interface between the computer and the EEPROM chip. It receives commands via the serial connection and either reads from the serial connection and writes to the EEPROM chip or vice versa. Data reading and writing is sequential and starts at address 0.

### Interfacing

There are two interfacing components included in this project. The graphical frontend depends on the [Serial Interface](https://github.com/Arc676/Serial-Interface) library (GPLv3). This is included as a submodule in the repository. In addition, the header file `Programmer/src/interface.h` defines the communication protocol between the microcontroller and the frontend.

### EEPROM Programmer

The graphical frontend uses [ImGui](https://github.com/ocornut/imgui) (MIT licensed) and allows the user to view the data on the EEPROM chip as well as modify it. The memory visualizer is also MIT licensed and available under [ImGui Club](https://github.com/ocornut/imgui_club).

#### Chip Presets

The EEPROM Programmer also offers some preset data for common PROM uses. Pin configuration diagrams and descriptions of the preset data can be found in `Programmer/Presets`.

## License

Project available under GPLv3. Arduino code based on EEPROM programmer by [Ben Eater](https://github.com/beneater/eeprom-programmer) available under MIT.
