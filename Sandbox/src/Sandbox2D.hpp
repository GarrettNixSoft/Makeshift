#pragma once

#include <Makeshift.hpp>

class Sandbox2D : public Makeshift::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(Makeshift::Timestep ts) override;
	virtual void onImGuiRender() override;
	virtual void onEvent(Makeshift::Event& e) override;

private:
	Makeshift::OrthographicCameraController cameraController;

	// TEMPORARY
	Makeshift::Ref<Makeshift::VertexArray> squareVA;
	Makeshift::Ref<Makeshift::Shader> flatColorShader;

	glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

};