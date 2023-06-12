#include "mkpch.hpp"
#include "VertexArray.hpp"

#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Makeshift {

	VertexArray* VertexArray::Create() {

		switch (Renderer::GetAPI()) {
			case RendererAPI::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

}