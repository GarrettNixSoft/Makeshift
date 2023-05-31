
#include <Makeshift.hpp>

class Sandbox : public Makeshift::Application {

public:
	Sandbox() {

	}

	~Sandbox() {

	}

};


Makeshift::Application* Makeshift::CreateApplication() {
	return new Sandbox();
}