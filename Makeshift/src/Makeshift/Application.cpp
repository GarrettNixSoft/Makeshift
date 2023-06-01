#include "mkpch.hpp"
#include "Application.hpp"

#include "Makeshift/Events/ApplicationEvent.hpp"
#include "Makeshift/Logger.hpp"

#include <GLFW/glfw3.h>

namespace Makeshift {

	Application::Application() {
		window = std::unique_ptr<Window>(Window::create());
	}

	Application::~Application() {

	}

	void Application::run() {

		while (running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			window->onUpdate();
		}

	}

}