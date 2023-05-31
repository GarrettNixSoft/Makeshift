#pragma once

#include "Core.hpp"

namespace Makeshift {

	class MK_API Application {

	public:
		Application();
		virtual ~Application();

		void run();

	};

	// To be defined by CLIENT
	Application* CreateApplication();

}