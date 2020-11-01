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

#include "programmer.h"

bool connected = false;
char device[200];
SerialConnection* sc;

unsigned char* data;
int bufferSize = 0;

MemoryEditor memEdit;

void glfwErrorCallback(int error, const char* description) {
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

int main() {
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit()) {
		return 1;
	}
	#ifdef __APPLE__
	const char* glslVersion = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#else
	const char* glslVersion = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	#endif

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(700, 600, "EEPROM Programmer", NULL, NULL);
	if (!window) {
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize OpenGL loader\n");
		return 1;
	}

	sc = initSerialConnection();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("EEPROM Programmer")) {
			if (ImGui::CollapsingHeader("Connection")) {
				ImGui::Text("Device Path");
				ImGui::SameLine();
				ImGuiInputTextFlags flags = connected ? ImGuiInputTextFlags_ReadOnly : 0;

				ImGui::InputText("##Device", device, 200, flags);
				ImGui::Text("Baud rate");
				ImGui::SameLine();

				static int selectedFreq = INDEX_9600;
				ImGui::Combo("##Freq", &selectedFreq, baudRates_s, NO_BAUD_RATES);
				sc->freq = baudRates_i[selectedFreq];
				ImGui::SameLine();

				if (!connected && ImGui::Button("Connect")) {
					connected = setupSerial(sc, device);
				} else if (connected && ImGui::Button("Disconnect")) {
					disconnectSerialConnection(sc);
					connected = false;
				}
			}

			if (connected) {
				if (ImGui::CollapsingHeader("Buffer")) {
					static int newSize = 0;
					ImGui::Text("Buffer Size");
					ImGui::SameLine();
					ImGui::InputInt("##BufSize", &newSize);
					if (ImGui::Button("Allocate")) {
						bufferSize = newSize;
						data = (unsigned char*)realloc(data, bufferSize);
					}
				}
				if (data && ImGui::CollapsingHeader("Chip Contents")) {
					memEdit.DrawContents(data, bufferSize);
					ImGui::SetCursorPosX(0);
					if (ImGui::TreeNode("Data Presets")) {
						static int address = 0;
						ImGui::Text("Starting Address");
						ImGui::SameLine();
						ImGui::InputInt("##Address", &address);
						if (address >= 0) {
							static int bytesNeeded = 0;
							if (ImGui::Button("Common Anode 7 Segment Decoder (0-F)")) {
								if (address + 16 <= bufferSize) {
									memcpy(data + address, commonAnode, 16);
								} else {
									bytesNeeded = 16;
								}
							} else if (ImGui::Button("Common Anode 7 Segment Decoder (0-9)")) {
								if (address + 10 <= bufferSize) {
									memcpy(data + address, commonAnode, 10);
								} else {
									bytesNeeded = 10;
								}
							} else if (ImGui::Button("Common Cathode 7 Segment Decoder (0-F)")) {
								if (address + 16 <= bufferSize) {
									memcpy(data + address, commonCathode, 16);
								} else {
									bytesNeeded = 16;
								}
							} else if (ImGui::Button("Common Cathode 7 Segment Decoder (0-9)")) {
								if (address + 10 <= bufferSize) {
									memcpy(data + address, commonCathode, 10);
								} else {
									bytesNeeded = 10;
								}
							} else if (ImGui::Button("Common Cathode 2-Digit Preset")) {
								if (address + 1024 <= bufferSize) {
									unsigned char* start = data + address;
									for (int i = 0; i < 16; i++) {
										memset(start + i * 16, commonCathode[i], 16);
										memcpy(start + 256 + i * 16, commonCathode, 16);
									}
									for (int i = -128; i < 128; i++) {
										unsigned char uc = (unsigned char)i;
										unsigned char sign = i < 0 ? 0x80 : 0x00;
										int abs = i < 0 ? -i : i;

										unsigned char sixteens = commonCathode[abs / 16];
										start[uc + 512] = sixteens | sign;

										unsigned char units = commonCathode[abs % 16];
										start[uc + 768] = units | sign;
									}
								} else {
									bytesNeeded = 1024;
								}
							} else if (ImGui::Button("Erase EEPROM (fill buffer with zeros)")) {
								memset(data, 0, bufferSize);
							} else if (ImGui::Button("Erase EEPROM (fill buffer with ones)")) {
								memset(data, 0xFF, bufferSize);
							}
							static char buf[150];
							if (bytesNeeded != 0) {
								ImGui::OpenPopup("insufficientSpace");
								sprintf(buf,
									"Error: This data needs %d bytes from the starting address but only %d are available. Maybe enlarge the buffer?",
									bytesNeeded,
									bufferSize - address);
								bytesNeeded = 0;
							}
							if (ImGui::BeginPopup("insufficientSpace")) {
								ImGui::Text(buf);
								ImGui::EndPopup();
							}
						} else {
							ImGui::Text("(Can't write to given address)");
						}
						ImGui::TreePop();
					}
				}
				if (ImGui::CollapsingHeader("Data Transfer")) {
					static int len = 0, offset = 0;
					ImGui::Text("Buffer Length");
					ImGui::SameLine();
					ImGui::InputInt("##BufLen", &len);

					ImGui::Text("Offset");
					ImGui::SameLine();
					ImGui::InputInt("##Offset", &offset);

					ImGui::Separator();
					if (0 < len && len + offset <= bufferSize && offset >= 0) {
						static int transferred = 0;
						static int keepStart = 1;
						ImGui::RadioButton("Maintain alignment of local buffer with EEPROM data", &keepStart, 1);
						ImGui::RadioButton("Transfer data to/from beginning of local buffer regardless of offset", &keepStart, 0);

						if (ImGui::Button("Write Buffer to Chip")) {
							transferred = writeEEPROM(sc, keepStart ? data + offset : data, len, offset);
						}
						if (ImGui::Button("Read Buffer from Chip")) {
							transferred = readEEPROM(sc, keepStart ? data + offset : data, len, offset);
						}
						ImGui::Text("Bytes transferred during last operation: %d", transferred);
					} else {
						ImGui::Text("(Cannot transfer data with current length or offset. Maybe enlarge the buffer?)");
					}
				}
			}

			if (ImGui::Button("Exit")) {
				break;
			}
		}
		ImGui::End();
		ImGui::Render();
		int displayW, displayH;
		glfwGetFramebufferSize(window, &displayW, &displayH);
		glViewport(0, 0, displayW, displayH);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();

	if (connected) {
		destroySerialConnection(sc);
	}
	if (data) {
		free(data);
	}

	return 0;
}
