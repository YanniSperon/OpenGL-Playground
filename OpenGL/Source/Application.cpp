#include "Global.h"
#include "Console.h"
#include "input.h"
#include "Object.h"
#include "Camera.h"
#include "Renderer.h"
#include "LinearRenderer.h"
#include "Light.h"
#include "Random.h"

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/Edited/ImGuiOpenGLRenderer.h"

#include <vector>
#include <string>

static void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		Console::FatalError("GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		Console::Error("GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
		break;
	case GL_DEBUG_SEVERITY_LOW:
		Console::Warning("GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		Console::Info("GL NOTIFICATION:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
		break;
	}
}

static double mouseXPos = 0.0;
static double mouseYPos = 0.0;

static double mouseSensitivity = 0.5;

static int windowWidth = 1920;
static int windowHeight = 1080;

static bool didMove = false;

int main() {
	/*
	Global::Initialize();

	Console::Assert(glfwInit(), "Failed GLFW Initialization!");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//////////////////////////////////////////////////////////////////////////////////////////////
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	Input* input = new Input();
	input->SetShouldCaptureKeyboardInput(true);
	input->SetShouldCaptureMouseInput(true);
	Console::Assert(window, "Failed Window Creation!");
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, input);
	//////////////////////////////////////////////////////////////////////////////////////////////
	GLenum err = glewInit();
	Console::Assert(err == GLEW_OK, "Failed GLEW Initialization - %s", reinterpret_cast<char const*>(glewGetErrorString(err)));
	//////////////////////////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_CCW);
	//////////////////////////////////////////////////////////////////////////////////////////////
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, 1);
	}
	glfwSwapInterval(0);
	glDebugMessageCallback(GLDebugMessageCallback, 0);
	glfwSetErrorCallback([](int error, const char* description)
		{
			Console::Error("GLFW Error (%i): \"%s\"", error, description);
		}
	);
	//////////////////////////////////////////////////////////////////////////////////////////////
	glfwSetCursorPosCallback(window, [](GLFWwindow* glfwWindow, double xPos, double yPos)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			input->MoveMouseTo(xPos, yPos);

			mouseXPos = xPos;
			mouseYPos = yPos;

			didMove = true;
		}
	);
	glfwSetMouseButtonCallback(window, [](GLFWwindow* glfwWindow, int button, int action, int mods)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			switch (action)
			{
			case GLFW_PRESS:
			{
				input->UpdateMouseButton(button, 1);
				break;
			}
			case GLFW_RELEASE:
			{
				input->UpdateMouseButton(button, 3);
				break;
			}
			}
		}
	);
	glfwSetKeyCallback(window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			switch (action)
			{
			case GLFW_PRESS:
			{
				input->UpdateKeyboardKey(key, 1);
				break;
			}
			case GLFW_RELEASE:
			{
				input->UpdateKeyboardKey(key, 3);
				break;
			}
			}
		}
	);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* glfwWindow, int width, int height)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			windowWidth = width;
			windowHeight = height;
		}
	);
	glfwSetCharCallback(window, [](GLFWwindow* glfwWindow, unsigned int keycode)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);
		}
	);
	glfwSetScrollCallback(window, [](GLFWwindow* glfwWindow, double xOffset, double yOffset)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);
		}
	);
	glfwSetCursorEnterCallback(window, [](GLFWwindow* glfwWindow, int entered)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			input->SetMouseWasBlocked(true);
		}
	);





	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	//GLuint VAO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);


	//Shader shader("Resources/Shaders/Lerp", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER);
	//shader.Bind();





	double timeConstant = 1.0;
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = lastTime;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTimeNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);
		lastTime = currentTime;
		double deltaTimeWithoutTimeFactor = deltaTimeNanoseconds.count() / 1000000000.0;
		double deltaTime = deltaTimeWithoutTimeFactor * timeConstant;

		glfwPollEvents();
		input->Prepare();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//shader.Bind();
		//shader.Set3Float("controlPoints[0]", -1.0, -1.0, 0.0);
		//shader.Set3Float("controlPoints[1]", -0.75, -0.25, 0.0);
		//shader.Set3Float("controlPoints[2]", -1.0, -2.0, 0.0);
		//shader.Set3Float("controlPoints[3]", +1.0, +1.0, 0.0);
		//shader.Set3Float("controlPoints[4]", +1.0, +1.0, 0.0);
		//shader.Set3Float("controlPoints[5]", +1.0, +1.0, 0.0);
		//shader.Set3Float("controlPoints[6]", +1.0, +1.0, 0.0);
		//shader.Set3Float("controlPoints[7]", +1.0, +1.0, 0.0);
		//glBindVertexArray(VAO);
		//
		//glPointSize(10);
		//glLineWidth(10);
		//glDrawArrays(GL_LINES, 0, 200);







		glfwSwapBuffers(window);

		input->Flush();
		Global::Update();
		didMove = false;
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	Global::Cleanup();
	*/

	Global::Initialize();

	Console::Assert(glfwInit(), "Failed GLFW Initialization!");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//////////////////////////////////////////////////////////////////////////////////////////////
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	Input* input = new Input();
	input->SetShouldCaptureKeyboardInput(true);
	input->SetShouldCaptureMouseInput(true);
	Console::Assert(window, "Failed Window Creation!");
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, input);
	//////////////////////////////////////////////////////////////////////////////////////////////
	GLenum err = glewInit();
	Console::Assert(err == GLEW_OK, "Failed GLEW Initialization - %s", reinterpret_cast<char const*>(glewGetErrorString(err)));
	//////////////////////////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_CCW);
	//////////////////////////////////////////////////////////////////////////////////////////////
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, 1);
	}
	glfwSwapInterval(0);
	glDebugMessageCallback(GLDebugMessageCallback, 0);
	glfwSetErrorCallback([](int error, const char* description)
		{
			Console::Error("GLFW Error (%i): \"%s\"", error, description);
		}
	);
	//////////////////////////////////////////////////////////////////////////////////////////////
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(windowWidth, windowHeight);
	static auto font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahoma.ttf", 14);
	io.IniFilename = nullptr;
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.KeyMap[ImGuiKey_Tab] = AD_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = AD_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = AD_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = AD_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = AD_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = AD_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = AD_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = AD_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = AD_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = AD_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = AD_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = AD_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = AD_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = AD_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = AD_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = AD_KEY_KP_ENTER;
	io.KeyMap[ImGuiKey_A] = AD_KEY_A;
	io.KeyMap[ImGuiKey_C] = AD_KEY_C;
	io.KeyMap[ImGuiKey_V] = AD_KEY_V;
	io.KeyMap[ImGuiKey_X] = AD_KEY_X;
	io.KeyMap[ImGuiKey_Y] = AD_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = AD_KEY_Z;
	ImGui_ImplOpenGL3_Init("#version 460");
	//////////////////////////////////////////////////////////////////////////////////////////////
	glfwSetCursorPosCallback(window, [](GLFWwindow* glfwWindow, double xPos, double yPos)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			input->MoveMouseTo(xPos, yPos);

			mouseXPos = xPos;
			mouseYPos = yPos;

			ImGuiIO& io = ImGui::GetIO();
			io.MousePos = ImVec2(xPos, yPos);

			didMove = true;
		}
	);
	glfwSetMouseButtonCallback(window, [](GLFWwindow* glfwWindow, int button, int action, int mods)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			ImGuiIO& io = ImGui::GetIO();

			switch (action)
			{
			case GLFW_PRESS:
			{
				input->UpdateMouseButton(button, 1);
				if (io.WantCaptureMouse) {
					io.MouseDown[button] = true;
				}
				break;
			}
			case GLFW_RELEASE:
			{
				input->UpdateMouseButton(button, 3);
				if (io.WantCaptureMouse) {
					io.MouseDown[button] = false;
				}
				break;
			}
			}
		}
	);
	glfwSetKeyCallback(window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			ImGuiIO& io = ImGui::GetIO();

			switch (action)
			{
			case GLFW_PRESS:
			{
				input->UpdateKeyboardKey(key, 1);
				if (io.WantCaptureKeyboard) {
					io.KeysDown[key] = true;
					
					io.KeyCtrl = io.KeysDown[AD_KEY_LEFT_CONTROL] || io.KeysDown[AD_KEY_RIGHT_CONTROL];
					io.KeyShift = io.KeysDown[AD_KEY_LEFT_SHIFT] || io.KeysDown[AD_KEY_RIGHT_SHIFT];
					io.KeyAlt = io.KeysDown[AD_KEY_LEFT_ALT] || io.KeysDown[AD_KEY_RIGHT_ALT];
					io.KeySuper = io.KeysDown[AD_KEY_LEFT_SUPER] || io.KeysDown[AD_KEY_RIGHT_SUPER];
				}
				break;
			}
			case GLFW_RELEASE:
			{
				input->UpdateKeyboardKey(key, 3);
				if (io.WantCaptureKeyboard) {
					io.KeysDown[key] = false;
				}
				break;
			}
			}
		}
	);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* glfwWindow, int width, int height)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			windowWidth = width;
			windowHeight = height;

			ImGuiIO& io = ImGui::GetIO();

			io.DisplaySize = ImVec2(windowWidth, windowHeight);
			io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		}
	);
	glfwSetCharCallback(window, [](GLFWwindow* glfwWindow, unsigned int keycode)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			ImGuiIO& io = ImGui::GetIO();

			if (io.WantCaptureKeyboard) {
				if (keycode > 0 && keycode < 0x10000) {
					io.AddInputCharacter((unsigned short)keycode);
				}
			}
		}
	);
	glfwSetScrollCallback(window, [](GLFWwindow* glfwWindow, double xOffset, double yOffset)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse) {
				io.MouseWheelH += xOffset;
				io.MouseWheel += yOffset;
			}
		}
	);
	glfwSetCursorEnterCallback(window, [](GLFWwindow* glfwWindow, int entered)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			input->SetMouseWasBlocked(true);
		}
	);





	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);







	GLfloat value, max_anisotropy = 8.0f; /* don't exceed this value...*/
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
	Console::Warning("Anisotropy value: %f", value);



	


	Renderer* renderer = new LinearRenderer();

	std::vector<Object*> objects = std::vector<Object*>();

	//objects.push_back(new Object("Resources/MassivePlane.obj", "Resources/Shaders/Wocky", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER, "", MATERIAL_NONE_TEXTURE, false));
	//objects[0]->SetTranslation(glm::vec3(0.0f, -3.0f, 0.0f));
	//objects[0]->SetScale(glm::vec3(1000.0f, 1.0f, 1000.0f));

	objects.push_back(new Object("Resources/MassivePlane.obj", "Resources/Shaders/PBR", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER, "Resources/Rust/Rust", MATERIAL_ALBEDO_TEXTURE | MATERIAL_NORMAL_TEXTURE | MATERIAL_METALLIC_TEXTURE | MATERIAL_ROUGHNESS_TEXTURE | MATERIAL_AO_TEXTURE, false));
	objects[0]->SetTranslation(glm::vec3(0.0f, -3.0f, 0.0f));
	objects[0]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	Camera* camera = new Camera();
	camera->SetSkybox("Resources/Skybox/Canyon.hdr", 1.6f);



	double timeConstant = 1.0;
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = lastTime;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTimeNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);
		lastTime = currentTime;
		double deltaTimeWithoutTimeFactor = deltaTimeNanoseconds.count() / 1000000000.0;
		double deltaTime = deltaTimeWithoutTimeFactor * timeConstant;

		

		glfwPollEvents();
		input->Prepare();

		

		// update and draw

		for (int i = 0; i < objects.size(); i++) {
			objects[i]->GenerateFinalTransformation(glm::mat4(1.0f));
		}

		if (input->GetKeyboardKeyHeld(AD_KEY_W)) {
			camera->MoveForward(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_A)) {
			camera->StrafeLeft(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_S)) {
			camera->MoveBackward(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_D)) {
			camera->StrafeRight(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_LEFT_CONTROL)) {
			camera->MoveDown(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_SPACE)) {
			camera->MoveUp(deltaTime);
		}
			
		if (didMove) {
			camera->LookAtMouse(mouseSensitivity, input->GetMousePositionX(), input->GetMousePositionY(), input->GetOldMousePositionX(), input->GetOldMousePositionY());
		}





		for (int i = 0; i < objects.size(); i++) {
			renderer->AddObject(objects[i]);
		}

		renderer->Flush(windowWidth, windowHeight, camera);











		glfwSwapBuffers(window);

		input->Flush();
		Global::Update();
		didMove = false;
	}





















	ImGui_ImplOpenGL3_Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	Global::Cleanup();
	
}