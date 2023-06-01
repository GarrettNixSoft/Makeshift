#include "mkpch.hpp"
#include "Application.hpp"

#include "Makeshift/Events/ApplicationEvent.hpp"
#include "Makeshift/Logger.hpp"

namespace Makeshift {

	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::run() {

		WindowResizeEvent e(1280, 720);
		if (e.isInCategory(EventCategoryApplication)) {
			MK_TRACE(e);
		}

		else if (e.isInCategory(EventCategoryInput)) {
			MK_TRACE(e);
		}

		while (true);

	}

}