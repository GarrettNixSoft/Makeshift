#include <Makeshift.hpp>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

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
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
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
			uniform mat4 transform;

			out vec3 fragPos;
			out vec4 vertColor;

			void main(void) {
				fragPos = position;
				vertColor = color;
				gl_Position = viewProjection * transform * vec4(position, 1.0);
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

		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;

			uniform mat4 viewProjection;
			uniform mat4 transform;

			out vec3 fragPos;

			void main(void) {
				fragPos = position;
				gl_Position = viewProjection * transform * vec4(position, 1.0);
			}
		)";

		std::string flatColorFragmentSrc = R"(
			#version 330 core

			in vec3 fragPos;

			uniform vec4 color;

			layout(location = 0) out vec4 outColor;

			void main(void) {
				outColor = color;
			}
		)";

		flatColorShader.reset(Makeshift::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));

	}

	void onUpdate(Makeshift::Timestep ts) override {

		//MK_TRACE("Delta time: {0}s ({1}ms)", ts.getSeconds(), ts.getMilliseconds());
		
		Makeshift::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Makeshift::RenderCommand::Clear();

		if (Makeshift::Input::isKeyPressed(MK_KEY_A)) {
			cameraPos.x -= cameraSpeed * ts;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_D)) {
			cameraPos.x += cameraSpeed * ts;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_W)) {
			cameraPos.y += cameraSpeed * ts;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_S)) {
			cameraPos.y -= cameraSpeed * ts;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_LEFT)) {
			cameraRotation += cameraRotationSpeed * ts;
		}
		if (Makeshift::Input::isKeyPressed(MK_KEY_RIGHT)) {
			cameraRotation -= cameraRotationSpeed * ts;
		}

		camera.setRotation(cameraRotation);
		camera.setPosition(cameraPos);

		Makeshift::Renderer::BeginScene(camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.1f, 1.0f);
		glm::vec4 blueColor(0.1f, 0.2f, 0.8f, 1.0f);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if ((x + y) % 2 == 0)
					flatColorShader->uploadUniformVec4("color", redColor);
				else
					flatColorShader->uploadUniformVec4("color", blueColor);
				Makeshift::Renderer::Submit(flatColorShader, squareVA, transform);
			}
		}

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

	std::shared_ptr<Makeshift::Shader> flatColorShader;
	std::shared_ptr<Makeshift::VertexArray> squareVA;

	Makeshift::OrthographicCamera camera;
	glm::vec3 cameraPos;
	float cameraSpeed = 2.0f;

	float cameraRotation = 0.0f;
	float cameraRotationSpeed = 90.0f;
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