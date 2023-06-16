#pragma once

#ifdef MK_PLATFORM_WINDOWS

extern Makeshift::Application* Makeshift::CreateApplication();

void main(int argc, char** argv) {

	Makeshift::Logger::init();
	MK_CORE_WARN("Initialized logging!");
	int a = 5;
	MK_INFO("Hello! Var={0}", a);

	auto app = Makeshift::CreateApplication();
	app->run();
	delete app;

}

#endif