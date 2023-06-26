#include "mkpch.hpp"
#include "Framebuffer.hpp"

#include "Makeshift/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace Makeshift {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLFramebuffer>(spec);
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

}