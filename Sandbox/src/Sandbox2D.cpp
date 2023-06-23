#include "Sandbox2D.hpp"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.hpp"

#include <chrono>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(1920.0f / 1080.0f) {

}

void Sandbox2D::onAttach() {
	MK_PROFILE_FUNCTION();

	texture = Makeshift::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::onDetach() {
	MK_PROFILE_FUNCTION();

	// Nothing
}

void Sandbox2D::onUpdate(Makeshift::Timestep ts) {

	MK_PROFILE_FUNCTION();

	// Update
	cameraController.onUpdate(ts);

	// Render
	{
		MK_PROFILE_SCOPE("Renderer Prep");
		Makeshift::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Makeshift::RenderCommand::Clear();
	}

	{
		MK_PROFILE_SCOPE("Render Scene");
		Makeshift::Renderer2D::BeginScene(cameraController.getCamera());

		Makeshift::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.0f), { 0.9f, 0.1f, 0.1f, 1.0f });
		Makeshift::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.1f, 0.9f, 0.1f, 1.0f });
		Makeshift::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, glm::radians(45.0f), texture, { 1.0f, 0.8f, 0.8f, 1.0f }, 10.0f);

		Makeshift::Renderer2D::DrawTriangle({ 1.0f, 1.0f }, { 0.6f, 0.6f }, { 0.1f, 0.3f, 0.9f, 1.0f });

		Makeshift::Renderer2D::EndScene();
	}

}

void Sandbox2D::onImGuiRender() {
	MK_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

	profileResults.clear();

	ImGui::End();
}

void Sandbox2D::onEvent(Makeshift::Event& e) {
	cameraController.onEvent(e);
}
