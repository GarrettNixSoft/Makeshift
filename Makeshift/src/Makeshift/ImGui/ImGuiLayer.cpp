#include "mkpch.hpp"
#include "ImGuiLayer.hpp"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"

#include "Makeshift/Application.hpp"

namespace Makeshift {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {

	}

	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::onAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: USE MAKESHIFT KEYCODES
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDetach() {

	}

	void ImGuiLayer::onUpdate() {

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

		float timeNow = (float)glfwGetTime();
		io.DeltaTime = lastTime > 0.0 ? (timeNow - lastTime) : (1.0f - 60.0f);
		lastTime = timeNow;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


#define BIND_EVENT_FN(x) std::bind(&ImGuiLayer::x, this, std::placeholders::_1)

	void ImGuiLayer::onEvent(Event& event) {

		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(onKeyPressed));
		dispatcher.dispatch<KeyReleasedEvent>(BIND_EVENT_FN(onKeyReleased));
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(onMouseButtonPressed));
		dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(onMouseButtonReleased));
		dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(onMouseMoved));
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(onMouseScrolled));

	}

	bool ImGuiLayer::onKeyPressed(KeyPressedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.getKeyCode()] = true;

		return true;
	}

	bool ImGuiLayer::onKeyReleased(KeyReleasedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.getKeyCode()] = false;

		return true;
	}

	bool ImGuiLayer::onMouseButtonPressed(MouseButtonPressedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.getMouseButton()] = true;

		return true;
	}

	bool ImGuiLayer::onMouseButtonReleased(MouseButtonReleasedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.getMouseButton()] = false;

		return true;
	}

	bool ImGuiLayer::onMouseMoved(MouseMovedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.getX(), event.getY());

		return true;
	}

	bool ImGuiLayer::onMouseScrolled(MouseScrolledEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += event.getXOffset();
		io.MouseWheel += event.getYOffset();

		return true;
	}


}