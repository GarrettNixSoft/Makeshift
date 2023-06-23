#include "mkpch.hpp"
#include "Buffer.hpp"
#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLBuffer.hpp"

namespace Makeshift {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexBuffer>(size);
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}
	
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

}