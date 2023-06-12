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

		vertexArray.reset(VertexArray::Create());

		float vertices[3 * 3 + 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		BufferLayout layout = {
			{ ShaderDataType::Vec3, "position" },
			{ ShaderDataType::Vec4, "color" }
		};

		vertexBuffer->setLayout(layout);
		vertexArray->addVertexBuffer(vertexBuffer);

		unsigned int indices[3] = {
			0, 1, 2
		};

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, 3));
		vertexArray->setIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		squareVA.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> squareVB; 
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->setLayout({ { ShaderDataType::Vec3, "position" } });
		squareVA->addVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		squareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			out vec3 fragPos;
			out vec4 vertColor;

			void main(void) {
				fragPos = position;
				gl_Position = vec4(position, 1.0);
				vertColor = color;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			in vec3 fragPos;
			in vec4 vertColor;

			layout(location = 0) out vec4 outColor;

			void main(void) {
				outColor = vec4(fragPos * 0.5 + 0.5, 1.0);
				outColor = vertColor;
			}
		)";

		shader.reset(Shader::Create(vertexSrc, fragmentSrc));

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;

			out vec3 fragPos;

			void main(void) {
				fragPos = position;
				gl_Position = vec4(position, 1.0);
			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core

			in vec3 fragPos;

			layout(location = 0) out vec4 outColor;

			void main(void) {
				outColor = vec4(1.0, 1.0, 0.0, 1.0);
			}
		)";

		squareShader.reset(Shader::Create(squareVertexSrc, squareFragmentSrc));

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

			squareShader->bind();
			squareVA->bind();
			glDrawElements(GL_TRIANGLES, squareVA->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

			shader->bind();
			vertexArray->bind();
			glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

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