#include <Makeshift.hpp>

#include "imgui/imgui.h"

class ExampleLayer : public Makeshift::Layer {
public:
	ExampleLayer() : Layer("Example"), camera(-1.6f, 1.6f, -0.9f, 0.9f), cameraPos(0.0f) {

		vertexArray.reset(Makeshift::VertexArray::Create());

		float vertices[3 * 3 + 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Makeshift::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Makeshift::VertexBuffer::Create(vertices, sizeof(vertices)));

		Makeshift::BufferLayout layout = {
			{ Makeshift::ShaderDataType::Vec3, "position" },
			{ Makeshift::ShaderDataType::Vec4, "color" }
		};

		vertexBuffer->setLayout(layout);
		vertexArray->addVertexBuffer(vertexBuffer);

		unsigned int indices[3] = {
			0, 1, 2
		};

		std::shared_ptr<Makeshift::IndexBuffer> indexBuffer;
		indexBuffer.reset(Makeshift::IndexBuffer::Create(indices, 3));
		vertexArray->setIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		squareVA.reset(Makeshift::VertexArray::Create());

		std::shared_ptr<Makeshift::VertexBuffer> squareVB;
		squareVB.reset(Makeshift::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->setLayout({ { Makeshift::ShaderDataType::Vec3, "position" } });
		squareVA->addVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<Makeshift::IndexBuffer> squareIB;
		squareIB.reset(Makeshift::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		squareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			uniform mat4 viewProjection;

			out vec3 fragPos;
			out vec4 vertColor;

			void main(void) {
				fragPos = position;
				vertColor = color;
				gl_Position = viewProjection * vec4(position, 1.0);
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

		shader.reset(Makeshift::Shader::Create(vertexSrc, fragmentSrc));

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;

			uniform mat4 viewProjection;

			out vec3 fragPos;

			void main(void) {
				fragPos = position;
				gl_Position = viewProjection * vec4(position, 1.0);
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

		squareShader.reset(Makeshift::Shader::Create(squareVertexSrc, squareFragmentSrc));

	}

	void onUpdate() override {
		
		Makeshift::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Makeshift::RenderCommand::Clear();

		if (Makeshift::Input::isKeyPressed(MK_KEY_A)) {
			cameraPos.x -= cameraSpeed;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_D)) {
			cameraPos.x += cameraSpeed;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_W)) {
			cameraPos.y += cameraSpeed;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_S)) {
			cameraPos.y -= cameraSpeed;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_LEFT)) {
			cameraRotation += cameraRotationSpeed;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_RIGHT)) {
			cameraRotation -= cameraRotationSpeed;
		}

		camera.setRotation(cameraRotation);
		camera.setPosition(cameraPos);

		Makeshift::Renderer::BeginScene(camera);

		Makeshift::Renderer::Submit(squareShader, squareVA);
		Makeshift::Renderer::Submit(shader, vertexArray);

		Makeshift::Renderer::EndScene();

	}

	void onImGuiRender() override {
		
	}

	void onEvent(Makeshift::Event& event) override {
		
		Makeshift::EventDispatcher dispatcher(event);
		dispatcher.dispatch<Makeshift::KeyPressedEvent>(MK_BIND_EVENT_FN(ExampleLayer::onKeyPressedEvent));

	}

	bool onKeyPressedEvent(Makeshift::KeyPressedEvent& event) {
		if (event.getKeyCode() == MK_KEY_ESCAPE) {
			MK_INFO("This should close the window");
		}

		return false;
	}

private:
	std::shared_ptr<Makeshift::Shader> shader;
	std::shared_ptr<Makeshift::VertexArray> vertexArray;

	std::shared_ptr<Makeshift::Shader> squareShader;
	std::shared_ptr<Makeshift::VertexArray> squareVA;

	Makeshift::OrthographicCamera camera;
	glm::vec3 cameraPos;
	float cameraSpeed = 0.02f;

	float cameraRotation = 0.0f;
	float cameraRotationSpeed = 0.2f;
};

class Sandbox : public Makeshift::Application {

public:
	Sandbox() {
		pushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}

};


Makeshift::Application* Makeshift::CreateApplication() {
	return new Sandbox();
}