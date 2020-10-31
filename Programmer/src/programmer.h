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

#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui_memory_editor.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "interface.h"

// Device baud rate control

#define NO_BAUD_RATES 28
#define INDEX_9600 12

const char* baudRates_s[] = {
	"50", "75",
	"110", "134", "150", "200", "300", "600",
	"1 200", "1 800", "2 400", "4 800", "9 600",
	"19 200", "38 400", "57 600",
	"115 200", "230 400", "460 800", "500 000", "576 000", "921 600",
	"1 000 000", "1 152 000", "1 500 000", "2 000 000", "3 000 000", "4 000 000"
};

const int baudRates_i[] = {
	B50, B75,
	B110, B134, B150, B200, B300, B600,
	B1200, B1800, B2400, B4800, B9600,
	B19200, B38400, B57600,
	B115200, B230400, B460800, B500000, B576000, B921600,
	B1000000, B1152000, B1500000, B2000000, B3000000, B4000000
};

// 7 segment decoders
// Bit order: Xgfedcba

const unsigned char commonAnode[] = {0x81, 0xcf, 0x92, 0x86, 0xcc, 0xa4, 0xa0, 0x8f, 0x80, 0x84, 0x88, 0xe0, 0xb1, 0xc2, 0xb0, 0xb8};
const unsigned char commonCathode[] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47};

#endif
