#include "mkpch.hpp"
#include "WindowsWindow.hpp"

namespace Makeshift {

	static bool GLFWinitialized = false;

	Window* Window::create(const WindowProperties& properties) {
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties) {
		init(properties);
	}

	WindowsWindow::~WindowsWindow() {

	}

	void WindowsWindow::init(const WindowProperties& properties) {
		data.title = properties.title;
		data.width = properties.width;
		data.height = properties.height;

		MK_CORE_INFO("Creating window {0}, size: {1} x {2}", properties.title, properties.width, properties.height);

		if (!GLFWinitialized) {

			int success = glfwInit();
			MK_CORE_ASSERT(success, "Could not initialize GLFW!");

			GLFWinitialized = true;

		}

		window = glfwCreateWindow((int)properties.width, (int)properties.height, data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		setVsync(true);
	}

	void WindowsWindow::shutdown() {
		glfwDestroyWindow(window);
	}

	void WindowsWindow::onUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowsWindow::setVsync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		data.vsync = enabled;
	}

	bool WindowsWindow::isVsync() const {
		return data.vsync;
	}

}