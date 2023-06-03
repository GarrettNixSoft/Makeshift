#pragma once

#include "Core.hpp"

#include "Window.hpp"
#include "Makeshift/LayerStack.hpp"
#include "Events/Event.hpp"
#include "Makeshift/Events/ApplicationEvent.hpp"

#include "Makeshift/ImGui/ImGuiLayer.hpp"

namespace Makeshift {

	class MK_API Application {

	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		inline Window& getWindow() { return *window; }
		inline static Application& Get() { return *instance; }
	private:
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> window;
		ImGuiLayer* imGuiLayer;
		bool running = true;

		LayerStack layerStack;
	private:
		static Application* instance;

	};

	// To be defined by CLIENT
	Application* CreateApplication();

}