#pragma once

#include "Core.hpp"
#include "Events/Event.hpp"
#include "Makeshift/Events/ApplicationEvent.hpp"

#include "Window.hpp"

namespace Makeshift {

	class MK_API Application {

	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& e);
	private:
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> window;
		bool running = true;

	};

	// To be defined by CLIENT
	Application* CreateApplication();

}