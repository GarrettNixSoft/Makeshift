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
	Makeshift::Renderer2D::ResetStats();
	{
		MK_PROFILE_SCOPE("Renderer Prep");
		Makeshift::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Makeshift::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 90.0f;

		MK_PROFILE_SCOPE("Render Scene");
		Makeshift::Renderer2D::BeginScene(cameraController.getCamera());

		Makeshift::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.9f, 0.1f, 0.1f, 1.0f });
		Makeshift::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.9f, 0.2f, 0.2f, 1.0f });
		Makeshift::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, squareColor);
		Makeshift::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, texture, { 1.0f, 0.8f, 0.8f, 1.0f }, 10.0f);
		Makeshift::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, texture, { 1.0f, 0.8f, 0.8f, 1.0f }, 20.0f);

		//Makeshift::Renderer2D::DrawTriangle({ 1.0f, 1.0f }, { 0.6f, 0.6f }, { 0.1f, 0.3f, 0.9f, 1.0f });

		Makeshift::Renderer2D::EndScene();

		Makeshift::Renderer2D::BeginScene(cameraController.getCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.2f, 0.7f };
				Makeshift::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}

		Makeshift::Renderer2D::EndScene();
	}

}

void Sandbox2D::onImGuiRender(Makeshift::Timestep ts) {
	MK_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Quad Color", glm::value_ptr(squareColor));

	profileResults.clear();

	ImGui::End();

	ImGui::Begin("Stats");

	auto stats = Makeshift::Renderer2D::GetStats();
	int fps = (int)(1000.0f / ts.getMilliseconds());

	ImGui::Text("-- Renderer2D --");
	ImGui::Text("Frame Time: %.3fms (%d fps)", ts.getMilliseconds(), fps);
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

void Sandbox2D::onEvent(Makeshift::Event& e) {
	cameraController.onEvent(e);
}
