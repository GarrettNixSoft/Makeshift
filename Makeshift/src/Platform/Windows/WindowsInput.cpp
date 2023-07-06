#include "mkpch.hpp"
#include "Makeshift/Core/Input.hpp"

#include <GLFW/glfw3.h>

#include "Makeshift/Core/Application.hpp"

namespace Makeshift {

	bool Input::IsKeyPressed(KeyCode key) {
		auto window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseButtonPressed(int button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}


	std::pair<float, float> Input::getMousePosition() {
		auto window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	float Input::GetMouseX() {
		auto [x, y] = getMousePosition();
		return x;
	}

	float Input::GetMouseY() {
		auto [x, y] = getMousePosition();
		return y;
	}

	float Input::getMouseScrollX() {
		auto window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
		// TODO
		return 0;
	}

	float Input::getMouseScrollY() {
		auto window = static_cast<GLFWwindow*>(Application::Get().getWindow().getNativeWindow());
		// TODO
		return 0;
	}

}