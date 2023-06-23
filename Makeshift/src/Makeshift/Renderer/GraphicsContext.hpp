#pragma once

#include <GLFW/glfw3.h>

namespace Makeshift {

	class GraphicsContext {
	public:
		virtual void init() = 0;
		virtual void swapBuffers() = 0;

		static Scope<GraphicsContext> Create(GLFWwindow* windowHandle);
	};

}