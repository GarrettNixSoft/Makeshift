#include "mkpch.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/Vulkan/VulkanShader.hpp"

#include <glad/glad.h>

namespace Makeshift {

	Ref<Shader> Shader::Create(const std::string& filePath) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(filePath);
			case RendererAPI::API::Vulkan:		return std::make_shared<VulkanShader>(filePath);
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}
	
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:		MK_CORE_ASSERT(false, "RendererAPI None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
			case RendererAPI::API::Vulkan:		return std::make_shared<VulkanShader>(name, vertexSrc, fragmentSrc);
		}

		MK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

	void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader) {
		MK_CORE_ASSERT(!exists(name), "Shader already exists!");
		shaders[name] = shader;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader) {
		const std::string& name = shader->getName();
		add(name, shader);
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filePath) {
		auto shader = Shader::Create(filePath);
		add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filePath) {
		auto shader = Shader::Create(filePath);
		add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::get(const std::string& name) {
		MK_CORE_ASSERT(exists(name), "Shader does not exist!");
		return shaders[name];
	}

	bool ShaderLibrary::exists(const std::string& name) const {
		return shaders.find(name) != shaders.end();
	}

}