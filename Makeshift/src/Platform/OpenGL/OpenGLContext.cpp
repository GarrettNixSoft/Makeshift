#include "mkpch.hpp"
#include "OpenGLContext.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Makeshift {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : windowHandle{ windowHandle } {
		MK_CORE_ASSERT(windowHandle, "windowHandle is null!")
	}

	void OpenGLContext::init() {
		MK_PROFILE_FUNCTION();

		glfwMakeContextCurrent(windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MK_CORE_ASSERT(status, "Failed to initialize Glad!");

		// Get OpenGL info
		const unsigned char* vendorStr = glGetString(GL_VENDOR);
		std::string vend(reinterpret_cast<char const*>(vendorStr));

		const unsigned char* rendererStr = glGetString(GL_RENDERER);
		std::string rend(reinterpret_cast<char const*>(rendererStr));

		const unsigned char* versionStr = glGetString(GL_VERSION);
		std::string vers(reinterpret_cast<char const*>(versionStr));

		MK_CORE_INFO("OpenGL Info:");
		MK_CORE_INFO("  Vendor: {0}", vend);
		MK_CORE_INFO("  Renderer: {0}", rend);
		MK_CORE_INFO("  Version: {0}", vers);
	}

	void OpenGLContext::swapBuffers() {
		MK_PROFILE_FUNCTION();

		glfwSwapBuffers(windowHandle);
	}

}