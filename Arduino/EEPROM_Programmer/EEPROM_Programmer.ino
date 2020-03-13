#define READ 0x00
#define WRITE 0xFF

#include <stddef.h>

void setup() {
	Serial.begin(9600);

	Serial.println("DEBUG: Reading EEPROM state");
	byte data[1024];
	readEEPROM(data, 1024, 0);
	char buf[80];
	for (int i = 0; i <= length; i++) {
		if (i % 16 == 0) {
			sprintf(buf, "%03x:\t", i);
		}
		sprintf(buf, "%s%02x ", buf, data[i]);
		if (i % 16 == 15 || i + 1 >= length) {
			Serial.println(buf);
		}
	}
}

void writeEEPROM(byte* data, size_t length, size_t offset) {
}

void readEEPROM(byte* dst, size_t length, size_t offset) {
}

void loop() {
	byte command;
	size_t length, offset;
	if (Serial.available() > 0) {
		Serial.readBytes(&command, 1);
		Serial.readBytes((byte*)&length, sizeof(size_t));
		Serial.readBytes((byte*)&offset, sizeof(size_t));
		byte* data = malloc(length);
		if (command == READ) {
			readEEPROM(data, length, offset);
			Serial.write(data, length);
		} else if (command == WRITE) {
			Serial.readBytes(data, length, offset);
			writeEEPROM(data, length);
		}
		free(data);
	}
}
