#pragma once

#include "Core.hpp"

#include "Makeshift/Core/Window.hpp"
#include "Makeshift/Core/LayerStack.hpp"
#include "Makeshift/Events/Event.hpp"
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

		inline Window& getWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;

	};

	// To be defined by CLIENT
	Application* CreateApplication();

}