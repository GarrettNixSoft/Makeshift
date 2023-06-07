#include "mkpch.hpp"
#include "Application.hpp"

#include <glad/glad.h>

#include "Input.hpp"

namespace Makeshift {

	Application* Application::instance = nullptr;

	Application::Application() {
		MK_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(MK_BIND_EVENT_FN(Application::onEvent));

		imGuiLayer = new ImGuiLayer();
		pushOverlay(imGuiLayer);

		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		unsigned int indices[3] = {
			0, 1, 2
		};

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;

			out vec3 fragPos;

			void main(void) {
				fragPos = position;
				gl_Position = vec4(position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			in vec3 fragPos;

			layout(location = 0) out vec4 outColor;

			void main(void) {
				outColor = vec4(fragPos * 0.5 + 0.5, 1.0);
			}
		)";

		shader.reset(new Shader(vertexSrc, fragmentSrc));

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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			shader->bind();
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : layerStack) {
				layer->onUpdate();
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