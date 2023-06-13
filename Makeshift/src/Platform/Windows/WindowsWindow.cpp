#include "mkpch.hpp"
#include "WindowsWindow.hpp"

#include "Makeshift/Events/KeyEvent.hpp"
#include "Makeshift/Events/MouseEvent.hpp"
#include "Makeshift/Events/ApplicationEvent.hpp"

#include "Makeshift/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLContext.hpp"
#include "Platform/Vulkan/VulkanContext.hpp"

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

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return;
			case RendererAPI::API::OpenGL:
				MK_CORE_INFO("Using OpenGL!");
				break;
			case RendererAPI::API::Vulkan:
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
				MK_CORE_INFO("Using Vulkan!");
		}

		window = glfwCreateWindow((int)properties.width, (int)properties.height, data.title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, &data);

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return;
			case RendererAPI::API::OpenGL:
				context = new OpenGLContext(window);
				break;
			case RendererAPI::API::Vulkan:
				context = new VulkanContext(window);
		}

		context->init();
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

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);

			data.eventCallback(event);
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
		context->swapBuffers();
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