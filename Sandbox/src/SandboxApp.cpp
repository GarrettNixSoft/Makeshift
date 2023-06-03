#include <Makeshift.hpp>

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