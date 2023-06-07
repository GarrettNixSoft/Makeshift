#pragma once

#include "Makeshift/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Makeshift {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* windowHandle;
	};

}