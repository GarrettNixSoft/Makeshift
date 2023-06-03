#include <Makeshift.hpp>

#include "imgui/imgui.h"

class ExampleLayer : public Makeshift::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void onUpdate() override {
		if (Makeshift::Input::isKeyPressed(MK_KEY_TAB)) {
			MK_TRACE("Tab key pressed");
		}
	}

	void onEvent(Makeshift::Event& event) override {
		if (event.getEventType() == Makeshift::EventType::KeyPressed) {
			Makeshift::KeyPressedEvent& e = (Makeshift::KeyPressedEvent&)event;
			MK_TRACE("{0}", (char) e.getKeyCode());
		}
	}

	void onImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}
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