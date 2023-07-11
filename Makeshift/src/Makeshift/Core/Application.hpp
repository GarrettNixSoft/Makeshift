#pragma once

#include "Core.hpp"

#include "Makeshift/Core/Window.hpp"
#include "Makeshift/Core/LayerStack.hpp"
#include "Makeshift/Events/Event.hpp"
#include "Makeshift/Events/ApplicationEvent.hpp"

#include "Makeshift/Core/Timestep.hpp"

#include "Makeshift/ImGui/ImGuiLayer.hpp"

namespace Makeshift {

	struct ApplicationCommandLineArgs {

		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const {
			MK_CORE_ASSERT(index < count, "arg index out of bounds");
			return args[index];
		}

	};

	class Application {

	public:
		Application(const std::string& name = "Makeshift App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		inline Window& getWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

		ApplicationCommandLineArgs getCommandLineArgs() const { return m_CommandLineArgs; }
	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;

	};

	// To be defined by CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);

}
