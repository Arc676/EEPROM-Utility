#define READ 0x00
#define WRITE 0xFF

#define ADDRESS_SER 2
#define ADDRESS_CLK 3
#define ADDRESS_LATCH 4

#define WRITE_ENABLE 12

#define READ_STATUS 10
#define WRITE_STATUS 11

#include <stddef.h>

const int dataPins[] = {A0, A1, A2, A3, A4, A5, 6, 7};
bool outputMode = false;

void setup() {
	Serial.begin(9600);

	pinMode(ADDRESS_CLK, OUTPUT);
	pinMode(ADDRESS_SER, OUTPUT);
	pinMode(ADDRESS_LATCH, OUTPUT);

	digitalWrite(ADDRESS_LATCH, LOW);
	digitalWrite(WRITE_ENABLE, HIGH);

	pinMode(WRITE_ENABLE, OUTPUT);

	pinMode(WRITE_STATUS, OUTPUT);
	pinMode(READ_STATUS, OUTPUT);

	for (int i = 0; i < 8; i++) {
		pinMode(dataPins[i], INPUT);
	}
}

void setAddress(int address, bool output) {
	shiftOut(ADDRESS_SER, ADDRESS_CLK, MSBFIRST, (address >> 8) | (output ? 0x00 : 0x80));
	shiftOut(ADDRESS_SER, ADDRESS_CLK, MSBFIRST, address);

	digitalWrite(ADDRESS_LATCH, HIGH);
	digitalWrite(ADDRESS_LATCH, LOW);
}

void writeEEPROM(byte* data, size_t length, size_t offset) {
	digitalWrite(WRITE_STATUS, HIGH);

	if (!outputMode) {
		for (int i = 0; i < 8; i++) {
			pinMode(dataPins[i], OUTPUT);
		}
		outputMode = true;
	}

	for (int address = (int)offset; address < (int)(offset + length); address++) {
		setAddress(address, false);
		byte b = data[address - offset];
		for (int i = 0; i < 8; i++) {
			digitalWrite(dataPins[i], b & 1);
			b >>= 1;
		}
		digitalWrite(WRITE_ENABLE, LOW);
		delayMicroseconds(1);
		digitalWrite(WRITE_ENABLE, HIGH);
		delay(10);
	}

	digitalWrite(WRITE_STATUS, LOW);
}

void readEEPROM(byte* dst, size_t length, size_t offset) {
	digitalWrite(READ_STATUS, HIGH);

	if (outputMode) {
		for (int i = 0; i < 8; i++) {
			pinMode(dataPins[i], INPUT);
		}
		outputMode = false;
	}

	for (int address = (int)offset; address < (int)(offset + length); address++) {
		setAddress(address, true);
		byte data = 0;
		for (int i = 7; i >= 0; i--) {
			data = (data << 1) | digitalRead(dataPins[i]);
		}
		dst[address - offset] = data;
	}

	digitalWrite(READ_STATUS, LOW);
}

void loop() {
	byte command;
	size_t length, offset;
	if (Serial.available() > 0) {
		Serial.readBytes(&command, 1);
		Serial.readBytes((byte*)&length, sizeof(size_t));
		Serial.readBytes((byte*)&offset, sizeof(size_t));
		byte* data = (byte*)malloc(length);
		if (command == READ) {
			readEEPROM(data, length, offset);
			Serial.write(data, length);
		} else if (command == WRITE) {
			Serial.readBytes(data, length);
			writeEEPROM(data, length, offset);
		}
		free(data);
	}
}
