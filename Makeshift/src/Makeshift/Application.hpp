#pragma once

#include "Core.hpp"

#include "Window.hpp"
#include "Makeshift/LayerStack.hpp"
#include "Events/Event.hpp"
#include "Makeshift/Events/ApplicationEvent.hpp"

#include "Makeshift/Core/Timestep.hpp"

#include "Makeshift/ImGui/ImGuiLayer.hpp"

namespace Makeshift {

	class Application {

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
	private:
		std::unique_ptr<Window> window;
		ImGuiLayer* imGuiLayer;
		bool running = true;

		LayerStack layerStack;

		float lastFrameTime = 0.0f;
	private:
		static Application* instance;

	};

	// To be defined by CLIENT
	Application* CreateApplication();

}