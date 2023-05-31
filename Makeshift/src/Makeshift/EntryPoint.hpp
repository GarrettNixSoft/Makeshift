#pragma once

#ifdef MK_PLATFORM_WINDOWS

extern Makeshift::Application* Makeshift::CreateApplication();

void main(int argc, char** argv) {

	auto app = Makeshift::CreateApplication();
	app->run();
	delete app;

}

#endif