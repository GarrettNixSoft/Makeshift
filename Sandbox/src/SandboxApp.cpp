#include <Makeshift.hpp>

class ExampleLayer : public Makeshift::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void onUpdate() override {
		MK_INFO("ExampleLayer::update");
	}

	void onEvent(Makeshift::Event& event) override {
		MK_TRACE("{0}", event);
	}
};

class Sandbox : public Makeshift::Application {

public:
	Sandbox() {
		pushLayer(new ExampleLayer());
		pushOverlay(new Makeshift::ImGuiLayer());
	}

	~Sandbox() {

	}

};


Makeshift::Application* Makeshift::CreateApplication() {
	return new Sandbox();
}