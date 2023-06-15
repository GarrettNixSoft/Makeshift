#include "mkpch.hpp"
#include "Application.hpp"

#include "Makeshift/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

namespace Makeshift {

	Application* Application::instance = nullptr;

	Application::Application() {
		MK_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(MK_BIND_EVENT_FN(Application::onEvent));
		//window->setVsync(false);

		Renderer::Init();

		imGuiLayer = new ImGuiLayer();
		pushOverlay(imGuiLayer);
	}

	Application::~Application() {

	}

	void Application::pushLayer(Layer* layer) {
		layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* overlay) {
		layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	void Application::onEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(MK_BIND_EVENT_FN(Application::onWindowClose));

		for (auto it = layerStack.end(); it != layerStack.begin(); ) {
			(*--it)->onEvent(e);
			if (e.handled) {
				break;
			}
		}
	}

	void Application::run() {

		while (running) {
			
			float time = (float) glfwGetTime(); // future: Platform::GetTime
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			for (Layer* layer : layerStack) {
				layer->onUpdate(timestep);
			}

			imGuiLayer->begin();
			for (Layer* layer : layerStack) {
				layer->onImGuiRender();
			}
			imGuiLayer->end();

			window->onUpdate();
		}

	}

	bool Application::onWindowClose(WindowCloseEvent& e) {

		running = false;
		return true;

	}

}