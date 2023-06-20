#pragma once

#ifdef MK_PLATFORM_WINDOWS

extern Makeshift::Application* Makeshift::CreateApplication();

void main(int argc, char** argv) {

	Makeshift::Logger::init();

	MK_PROFILE_BEGIN_SESSION("Startup", "MakeshiftProfile-Startup.json");
	auto app = Makeshift::CreateApplication();
	MK_PROFILE_END_SESSION();

	MK_PROFILE_BEGIN_SESSION("Startup", "MakeshiftProfile-Runtime.json");
	app->run();
	MK_PROFILE_END_SESSION();

	MK_PROFILE_BEGIN_SESSION("Startup", "MakeshiftProfile-Shutdown.json");
	delete app;
	MK_PROFILE_END_SESSION();

}

#endif