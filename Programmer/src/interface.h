// Copyright (C) 2020 Arc676/Alessandro Vinciguerra

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation (version 3)

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// See README and LICENSE for more details.

#ifndef INTERFACE_H
#define INTERFACE_H

#include <stddef.h>

#include "serialcomm.h"

#define READ 0x00
#define WRITE 0xFF

#define CMDSIZE 1 + 2 * sizeof(uint32_t)

/**
 * Reads data from the EEPROM chip
 * @param conn Serial connection to use
 * @param buf Buffer in which to store the read data
 * @param len Number of bytes to read
 * @param offset Starting address from which to read
 * @return Number of bytes received from serial port
 */
size_t readEEPROM(SerialConnection* conn, unsigned char* buf, uint32_t len, uint32_t offset) {
	unsigned char cmd = READ;
	writeSerial(conn, &cmd, 1);
	writeSerial(conn, (unsigned char*)&len, sizeof(uint32_t));
	writeSerial(conn, (unsigned char*)&offset, sizeof(uint32_t));
	size_t read = 0;
	while (read < len) {
		read += readSerial(conn, buf + read, len - read);
	}
	return read;
}

/**
 * Writes data to the EEPROM chip
 * @param conn Serial connection to use
 * @param buf Buffer with data to write to chip
 * @param len Number of bytes to write
 * @param offset Starting address from which to write
 * @return Number of bytes written to serial port
 */
size_t writeEEPROM(SerialConnection* conn, unsigned char* buf, uint32_t len, uint32_t offset) {
	unsigned char cmd = WRITE;
	writeSerial(conn, &cmd, 1);
	writeSerial(conn, (unsigned char*)&len, sizeof(uint32_t));
	writeSerial(conn, (unsigned char*)&offset, sizeof(uint32_t));
	return writeSerial(conn, buf, len);
}

#endif
