#include "Sandbox2D.hpp"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(1920.0f / 1080.0f) {

}

void Sandbox2D::onAttach() {

	squareVA = Makeshift::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Makeshift::Ref<Makeshift::VertexBuffer> squareVB;
	squareVB.reset(Makeshift::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

	squareVB->setLayout({
		{ Makeshift::ShaderDataType::Vec3, "position" }
	});
	squareVA->addVertexBuffer(squareVB);

	unsigned int squareIndices[6] = {
		0, 1, 2, 2, 3, 0
	};

	Makeshift::Ref<Makeshift::IndexBuffer> squareIB;
	squareIB.reset(Makeshift::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

	squareVA->setIndexBuffer(squareIB);

	flatColorShader = Makeshift::Shader::Create("assets/shaders/flatColor.glsl");

}

void Sandbox2D::onDetach() {
	// Nothing
}

void Sandbox2D::onUpdate(Makeshift::Timestep ts) {

	// Update
	cameraController.onUpdate(ts);

	// Render
	Makeshift::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Makeshift::RenderCommand::Clear();

	Makeshift::Renderer::BeginScene(cameraController.getCamera());

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	std::dynamic_pointer_cast<Makeshift::OpenGLShader>(flatColorShader)->bind();
	std::dynamic_pointer_cast<Makeshift::OpenGLShader>(flatColorShader)->uploadUniformVec4("u_Color", squareColor);

	// Render Colored Quad
	Makeshift::Renderer::Submit(flatColorShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Makeshift::Renderer::EndScene();

}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Makeshift::Event& e) {
	cameraController.onEvent(e);
}
