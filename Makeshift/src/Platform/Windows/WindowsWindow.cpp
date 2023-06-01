#include "mkpch.hpp"
#include "WindowsWindow.hpp"

#include "Makeshift/Events/KeyEvent.hpp"
#include "Makeshift/Events/MouseEvent.hpp"
#include "Makeshift/Events/ApplicationEvent.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Makeshift {

	static bool GLFWinitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		MK_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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

			glfwSetErrorCallback(GLFWErrorCallback);

			GLFWinitialized = true;

		}

		window = glfwCreateWindow((int)properties.width, (int)properties.height, data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);

		int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		MK_CORE_ASSERT(status, "Failed to initialize Glad!");

		glfwSetWindowUserPointer(window, &data);
		setVsync(true);

		// Set GLFW callbacks for events
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1); // TODO extract repeat count
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double dx, double dy) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)dx, (float)dy);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)x, (float)y);
			data.eventCallback(event);
		});

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