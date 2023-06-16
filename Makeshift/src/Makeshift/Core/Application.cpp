#include "mkpch.hpp"
#include "Application.hpp"

#include "Makeshift/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

namespace Makeshift {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		MK_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(MK_BIND_EVENT_FN(Application::onEvent));
		//window->setVsync(false);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {

	}

	void Application::pushLayer(Layer* layer) {
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* overlay) {
		m_LayerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	void Application::onEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(MK_BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(MK_BIND_EVENT_FN(Application::onWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->onEvent(e);
			if (e.handled) {
				break;
			}
		}
	}

	void Application::run() {

		while (m_Running) {
			
			float time = (float) glfwGetTime(); // future: Platform::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Only update the layer stack when not minimized
			if (!m_Minimized) {
				for (Layer* layer : m_LayerStack) {
					layer->onUpdate(timestep);
				}
			}

			// ImGui might be external to the window, so ignore minimized state for it
			m_ImGuiLayer->begin();
			for (Layer* layer : m_LayerStack) {
				layer->onImGuiRender();
			}
			m_ImGuiLayer->end();

			m_Window->onUpdate();
		}

	}

	bool Application::onWindowClose(WindowCloseEvent& e) {

		m_Running = false;
		return true;

	}

	bool Application::onWindowResize(WindowResizeEvent& e) {

		if (e.getWidth() == 0 || e.getHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.getWidth(), e.getHeight());

		return false;

	}

}