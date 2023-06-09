#include "mkpch.hpp"
#include "Application.hpp"

#include <glad/glad.h>

#include "Input.hpp"

namespace Makeshift {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {

		switch (type) {
			case ShaderDataType::Float:	return GL_FLOAT;
			case ShaderDataType::Int:	return GL_INT;
			case ShaderDataType::Bool:	return GL_BOOL;
			case ShaderDataType::Vec2:	return GL_FLOAT;
			case ShaderDataType::Vec3:	return GL_FLOAT;
			case ShaderDataType::Vec4:	return GL_FLOAT;
			case ShaderDataType::Mat3:	return GL_FLOAT;
			case ShaderDataType::Mat4:	return GL_FLOAT;
			case ShaderDataType::Vec2i:	return GL_INT;
			case ShaderDataType::Vec3i:	return GL_INT;
			case ShaderDataType::Vec4i:	return GL_INT;
		}

		MK_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

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

		float vertices[3 * 3 + 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		{
			BufferLayout layout = {
				{ ShaderDataType::Vec3, "position" },
				{ ShaderDataType::Vec4, "color" }
			};

			vertexBuffer->setLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout) {

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.offset);
			index++;

		}

		unsigned int indices[3] = {
			0, 1, 2
		};

		indexBuffer.reset(IndexBuffer::Create(indices, 3));

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
			glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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