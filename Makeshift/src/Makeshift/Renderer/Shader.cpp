#include "mkpch.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"

#include <glad/glad.h>

namespace Makeshift {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

}