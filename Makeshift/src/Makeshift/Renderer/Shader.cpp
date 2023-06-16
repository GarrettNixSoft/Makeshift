#include "mkpch.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/Vulkan/VulkanShader.hpp"

#include <glad/glad.h>

namespace Makeshift {

	Shader* Shader::Create(const std::string& filePath) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGLShader(filePath);
			case RendererAPI::API::Vulkan:		return new VulkanShader(filePath);
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}
	
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGLShader(vertexSrc, fragmentSrc);
			case RendererAPI::API::Vulkan:		return new VulkanShader(vertexSrc, fragmentSrc);
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

}