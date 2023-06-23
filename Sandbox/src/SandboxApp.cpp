#include <Makeshift.hpp>

// ---------------- Entry Point ---------------------
#include <Makeshift/Core/EntryPoint.hpp>
// --------------------------------------------------

#include "Platform/OpenGL/OpenGLShader.hpp"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.hpp"

class ExampleLayer : public Makeshift::Layer {
public:
	ExampleLayer() : Layer("Example"), cameraController(1920.0f / 1080.0f) {

		vertexArray = Makeshift::VertexArray::Create();

		float vertices[3 * 3 + 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		Makeshift::Ref<Makeshift::VertexBuffer> vertexBuffer;
		vertexBuffer == Makeshift::VertexBuffer::Create(vertices, sizeof(vertices));

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
		indexBuffer = Makeshift::IndexBuffer::Create(indices, 3);
		vertexArray->setIndexBuffer(indexBuffer);

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		squareVA = Makeshift::VertexArray::Create();

		Makeshift::Ref<Makeshift::VertexBuffer> squareVB;
		squareVB = Makeshift::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({
			{ Makeshift::ShaderDataType::Vec3, "position" },
			{ Makeshift::ShaderDataType::Vec2, "texCoord" }
		});
		squareVA->addVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		Makeshift::Ref<Makeshift::IndexBuffer> squareIB;
		squareIB = Makeshift::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		squareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 fragPos;
			out vec4 vertColor;

			void main(void) {
				fragPos = position;
				vertColor = color;
				gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);
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

		triangleShader = Makeshift::Shader::Create("VertPosColor", vertexSrc, fragmentSrc);

		

		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 fragPos;

			void main(void) {
				fragPos = position;
				gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);
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

		flatColorShader = Makeshift::Shader::Create("FlatColor", flatColorVertexSrc, flatColorFragmentSrc);

		auto textureShader = shaderLibrary.load("assets/shaders/texture.glsl");

		texture = Makeshift::Texture2D::Create("assets/textures/checkerboard.png");
		makeshiftTexture = Makeshift::Texture2D::Create("assets/textures/makeshift.png");

		std::dynamic_pointer_cast<Makeshift::OpenGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<Makeshift::OpenGLShader>(flatColorShader)->uploadUniformInt("u_Texture", 0);

	}

	void onUpdate(Makeshift::Timestep ts) override {

		//MK_TRACE("Delta time: {0}s ({1}ms)", ts.getSeconds(), ts.getMilliseconds());

		// Update
		cameraController.onUpdate(ts);
		
		// Render
		Makeshift::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Makeshift::RenderCommand::Clear();

		Makeshift::Renderer::BeginScene(cameraController.getCamera());

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

		auto textureShader = shaderLibrary.get("texture");

		// Render Textured Quad
		texture->bind();
		Makeshift::Renderer::Submit(textureShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		makeshiftTexture->bind();
		Makeshift::Renderer::Submit(textureShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Render Triangle
		// Makeshift::Renderer::Submit(triangleShader, vertexArray);

		Makeshift::Renderer::EndScene();

	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));
		ImGui::SliderFloat("Square Spacing", &squareSpacing, 0.01f, 0.1f);
		ImGui::End();
	}

	void onEvent(Makeshift::Event& e) override {
		
		Makeshift::EventDispatcher dispatcher(e);
		dispatcher.dispatch<Makeshift::KeyPressedEvent>(MK_BIND_EVENT_FN(ExampleLayer::onKeyPressedEvent));

		cameraController.onEvent(e);

	}

	bool onKeyPressedEvent(Makeshift::KeyPressedEvent& event) {
		if (event.getKeyCode() == MK_KEY_ESCAPE) {
			MK_INFO("This should close the window");
		}

		return false;
	}

private:
	Makeshift::ShaderLibrary shaderLibrary;

	Makeshift::Ref<Makeshift::Shader> triangleShader;
	Makeshift::Ref<Makeshift::VertexArray> vertexArray;

	Makeshift::Ref<Makeshift::Shader> flatColorShader;
	Makeshift::Ref<Makeshift::VertexArray> squareVA;

	Makeshift::Ref<Makeshift::Texture2D> texture;
	Makeshift::Ref<Makeshift::Texture2D> makeshiftTexture;

	Makeshift::OrthographicCameraController cameraController;

	glm::vec3 squareColor = { 0.2f, 0.3f, 0.8f };

	float squareSpacing = 0.01f;
};

class Sandbox : public Makeshift::Application {

public:
	Sandbox() {
		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}

};


Makeshift::Application* Makeshift::CreateApplication() {
	return new Sandbox();
}