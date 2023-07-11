#include "mkpch.hpp"
#include "UniformBuffer.hpp"

#include "Makeshift/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLUniformBuffer.hpp"

namespace Makeshift {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:	MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLUniformBuffer>(size, binding);
				// TODO: Vulkan
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}

}
