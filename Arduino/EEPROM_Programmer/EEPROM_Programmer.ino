#define READ 0x00
#define WRITE 0xFF

void setup() {
	Serial.begin(9600);
}

void writeEEPROM(byte* data, size_t length) {
}

void readEEPROM(byte* dst, size_t length) {
}

void loop() {
	byte command;
	int length;
	if (Serial.available() > 0) {
		Serial.readBytes(&command, 1);
		Serial.readBytes((byte*)&length, 4);
		if (command == READ) {
			byte* data = malloc(length);
			readEEPROM(data, length);
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
			free(data);
		} else if (command == WRITE) {
			byte* data = malloc(length);
			Serial.readBytes(data, length);
			writeEEPROM(data, length);
			free(data);
		}
	}
}
