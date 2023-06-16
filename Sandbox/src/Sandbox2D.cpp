#include "Sandbox2D.hpp"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(1920.0f / 1080.0f) {

}

void Sandbox2D::onAttach() {

	

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

	Makeshift::Renderer2D::BeginScene(cameraController.getCamera());

	Makeshift::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.9f, 0.1f, 0.1f, 1.0f });

	Makeshift::Renderer2D::EndScene();

	// TODO Add functions Shader::SetMat4, Shader::SetVec4 (abstract shader types away, get rid of OpenGL dependency here)
	//std::dynamic_pointer_cast<Makeshift::OpenGLShader>(flatColorShader)->bind();
	//std::dynamic_pointer_cast<Makeshift::OpenGLShader>(flatColorShader)->uploadUniformVec4("u_Color", squareColor);

}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Makeshift::Event& e) {
	cameraController.onEvent(e);
}
