#include "mkpch.hpp"
#include "VertexArray.hpp"

#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLVertexArray.hpp"

#include <memory>

namespace Makeshift {

	Ref<VertexArray> VertexArray::Create() {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLVertexArray>();
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

}