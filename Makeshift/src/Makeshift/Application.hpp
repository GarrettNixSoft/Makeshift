#pragma once

#include "Core.hpp"
#include "Events/Event.hpp"
#include "Window.hpp"

namespace Makeshift {

	class MK_API Application {

	public:
		Application();
		virtual ~Application();

		void run();
	private:
		std::unique_ptr<Window> window;
		bool running = true;

	};

	// To be defined by CLIENT
	Application* CreateApplication();

}