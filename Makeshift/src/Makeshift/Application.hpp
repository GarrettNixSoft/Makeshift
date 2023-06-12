#pragma once

#include "Core.hpp"

#include "Window.hpp"
#include "Makeshift/LayerStack.hpp"
#include "Events/Event.hpp"
#include "Makeshift/Events/ApplicationEvent.hpp"

#include "Makeshift/ImGui/ImGuiLayer.hpp"

#include "Makeshift/Renderer/Shader.hpp"
#include "Makeshift/Renderer/Buffer.hpp"
#include "Makeshift/Renderer/VertexArray.hpp"

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

		std::shared_ptr<Shader> shader;
		std::shared_ptr<VertexArray> vertexArray;

		std::shared_ptr<Shader> squareShader;
		std::shared_ptr<VertexArray> squareVA;

	private:
		static Application* instance;

	};

	// To be defined by CLIENT
	Application* CreateApplication();

}