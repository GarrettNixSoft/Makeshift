#include "mkpch.hpp"
#include "GraphicsContext.hpp"

#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLContext.hpp"
#include "Platform/Vulkan/VulkanContext.hpp"

namespace Makeshift {

	Scope<GraphicsContext> GraphicsContext::Create(GLFWwindow* windowHandle) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateScope<OpenGLContext>(windowHandle);
				break;
			case RendererAPI::API::Vulkan:
				return CreateScope<VulkanContext>(windowHandle);
		}

		MK_CORE_ASSERT(false, "Unknown RenderAPI");
		return nullptr;

	}

}