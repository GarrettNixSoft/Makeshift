#include "mkpch.hpp"
#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace Makeshift {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() {
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(BIND_EVENT_FN(onEvent));
	}

	Application::~Application() {

	}

	void Application::onEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

		MK_CORE_TRACE("{0}", e);
	}

	void Application::run() {

		while (running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			window->onUpdate();
		}

	}

	bool Application::onWindowClose(WindowCloseEvent& e) {

		running = false;
		return true;

	}

}