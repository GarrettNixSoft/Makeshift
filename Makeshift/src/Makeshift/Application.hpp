#pragma once

#include "Core.hpp"

#include "Window.hpp"
#include "Makeshift/LayerStack.hpp"
#include "Events/Event.hpp"
#include "Makeshift/Events/ApplicationEvent.hpp"


namespace Makeshift {

	class MK_API Application {

	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
	private:
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> window;
		bool running = true;

		LayerStack layerStack;

	};

	// To be defined by CLIENT
	Application* CreateApplication();

}