#include <Makeshift.hpp>

#include "Platform/OpenGL/OpenGLShader.hpp"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Makeshift::Layer {
public:
	ExampleLayer() : Layer("Example"), camera(-1.6f, 1.6f, -0.9f, 0.9f), cameraPos(0.0f) {

		vertexArray.reset(Makeshift::VertexArray::Create());

		float vertices[3 * 3 + 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		Makeshift::Ref<Makeshift::VertexBuffer> vertexBuffer;
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

		Makeshift::Ref<Makeshift::IndexBuffer> indexBuffer;
		indexBuffer.reset(Makeshift::IndexBuffer::Create(indices, 3));
		vertexArray->setIndexBuffer(indexBuffer);

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		squareVA.reset(Makeshift::VertexArray::Create());

		Makeshift::Ref<Makeshift::VertexBuffer> squareVB;
		squareVB.reset(Makeshift::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->setLayout({
			{ Makeshift::ShaderDataType::Vec3, "position" },
			{ Makeshift::ShaderDataType::Vec2, "texCoord" }
		});
		squareVA->addVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		Makeshift::Ref<Makeshift::IndexBuffer> squareIB;
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

			uniform vec3 color;

			layout(location = 0) out vec4 outColor;

			void main(void) {
				outColor = vec4(color, 1.0);
			}
		)";

		flatColorShader.reset(Makeshift::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));

		std::string textureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec2 texCoord;

			uniform mat4 viewProjection;
			uniform mat4 transform;

			out vec2 fragTexCoord;

			void main(void) {
				fragTexCoord = texCoord;
				gl_Position = viewProjection * transform * vec4(position, 1.0);
				
			}
		)";

		std::string textureFragmentSrc = R"(
			#version 330 core

			in vec2 fragTexCoord;

			layout(location = 0) out vec4 outColor;

			uniform sampler2D u_Texture;

			void main(void) {
				outColor = texture(u_Texture, fragTexCoord);
			}
		)";

		textureShader.reset(Makeshift::Shader::Create(textureVertexSrc, textureFragmentSrc));

		texture = Makeshift::Texture2D::Create("assets/textures/nixon.png");

		std::dynamic_pointer_cast<Makeshift::OpenGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<Makeshift::OpenGLShader>(flatColorShader)->uploadUniformInt("u_Texture", 0);

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

		std::dynamic_pointer_cast<Makeshift::OpenGLShader>(flatColorShader)->bind();
		std::dynamic_pointer_cast<Makeshift::OpenGLShader>(flatColorShader)->uploadUniformVec3("color", squareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * (0.1f + squareSpacing), y * (0.1f + squareSpacing), 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Makeshift::Renderer::Submit(flatColorShader, squareVA, transform);
			}
		}

		// Render Textured Quad
		texture->bind();
		Makeshift::Renderer::Submit(textureShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Render Triangle
		//Makeshift::Renderer::Submit(shader, vertexArray);

		Makeshift::Renderer::EndScene();

	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));
		ImGui::SliderFloat("Square Spacing", &squareSpacing, 0.01f, 0.1f);
		ImGui::End();
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
	Makeshift::Ref<Makeshift::Shader> shader, textureShader;
	Makeshift::Ref<Makeshift::VertexArray> vertexArray;

	Makeshift::Ref<Makeshift::Shader> flatColorShader;
	Makeshift::Ref<Makeshift::VertexArray> squareVA;

	Makeshift::Ref<Makeshift::Texture2D> texture;

	Makeshift::OrthographicCamera camera;
	glm::vec3 cameraPos;
	float cameraSpeed = 2.0f;

	float cameraRotation = 0.0f;
	float cameraRotationSpeed = 90.0f;

	glm::vec3 squareColor = { 0.2f, 0.3f, 0.8f };

	float squareSpacing = 0.01f;
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