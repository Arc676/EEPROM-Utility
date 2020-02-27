#include "programmer.h"

int connected = 0;
char device[200];
SerialConnection* sc;

int bytesSent = 0, bytesRead = 0;
int bytesToRead = 0;
char sendData[200], readData[200];

typedef enum SendState {
	SENT, SEND_FAILED, WAITING
} SendState;

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

	GLFWwindow* window = glfwCreateWindow(500, 500, "EEPROM Programmer", NULL, NULL);
	if (!window) {
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize OpenGL loader\n");
		return 1;
	}

	memset(device, 0, 200);
	memset(sendData, 0, 200);
	memset(readData, 0, 200);

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
		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("EEPROM Programmer")) {
			if (ImGui::CollapsingHeader("Connection")) {
				ImGui::Text("Device Path");
				ImGui::SameLine();
				ImGuiInputTextFlags flags = 0;
				if (connected) {
					flags = ImGuiInputTextFlags_ReadOnly;
				}
				ImGui::InputText("##Device", device, 200, flags);
				ImGui::Text("Baud rate");
				ImGui::SameLine();
				ImGui::InputInt("##Freq", &(sc->freq));
				ImGui::SameLine();
				if (!connected && ImGui::Button("Connect")) {
					connected = setupSerial(sc, device);
				} else if (connected && ImGui::Button("Disconnect")) {
					disconnectSerialConnection(sc);
					connected = 0;
				}
			}

			if (connected) {
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

	return 0;
}
