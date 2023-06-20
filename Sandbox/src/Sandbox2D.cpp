#include "Sandbox2D.hpp"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.hpp"

#include <chrono>

// ================================ PROFILING ================================
template<typename Fn>
class Timer {
public:
	Timer(const char* name, Fn&& func) : m_Name(name), m_Func(func), m_Stopped(false) {
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}
	
	~Timer() {
		if (!m_Stopped)
			stop();
	}

	void stop() {
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;

		m_Func({ m_Name, duration });
	}

private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
	bool m_Stopped;

	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { profileResults.push_back(profileResult); })
// ===========================================================================





Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(1920.0f / 1080.0f) {

}

void Sandbox2D::onAttach() {

	texture = Makeshift::Texture2D::Create("assets/textures/checkerboard.png");

}

void Sandbox2D::onDetach() {
	// Nothing
}

void Sandbox2D::onUpdate(Makeshift::Timestep ts) {

	PROFILE_SCOPE("Sandbox2D::onUpdate");

	// Update
	{
		PROFILE_SCOPE("CameraController::onUpdate");
		cameraController.onUpdate(ts);
	}

	// Render
	{
		PROFILE_SCOPE("Renderer Prep");
		Makeshift::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Makeshift::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Render Scene");
		Makeshift::Renderer2D::BeginScene(cameraController.getCamera());

		Makeshift::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.9f, 0.1f, 0.1f, 1.0f }, 45.0f);
		Makeshift::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.1f, 0.9f, 0.1f, 1.0f });
		Makeshift::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, texture, { 1.0f, 0.8f, 0.8f, 1.0f }, 0.0f, 10.0f);

		Makeshift::Renderer2D::EndScene();
	}

}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

	for (auto& result : profileResults) {
		char label[50];
		strncpy_s(label, "%.3fms ", 50);
		strncat_s(label, result.name, 43);
		ImGui::Text(label, result.time);
	}

	profileResults.clear();

	ImGui::End();
}

void Sandbox2D::onEvent(Makeshift::Event& e) {
	cameraController.onEvent(e);
}
