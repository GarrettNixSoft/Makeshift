#pragma once

#include "Makeshift/Renderer/Shader.hpp"

#include <glm/glm.hpp>

// TODO: Remove, this is bad :)
typedef unsigned int GLenum;
typedef int GLint;

namespace Makeshift {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual const std::string& getName() const override { return m_Name; };

		void uploadUniformFloat(const std::string& name, float value);
		void uploadUniformVec2(const std::string& name, const glm::vec2& vector);
		void uploadUniformVec3(const std::string& name, const glm::vec3& vector);
		void uploadUniformVec4(const std::string& name, const glm::vec4& vector);

		void uploadUniformInt(const std::string& name, int value);
		void uploadUniformVec2i(const std::string& name, const glm::ivec2& vector);
		void uploadUniformVec3i(const std::string& name, const glm::ivec4& vector);
		void uploadUniformVec4i(const std::string& name, const glm::ivec4& vector);

		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string readFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> preprocess(const std::string& source);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		GLint getUniformLocation(const std::string& name);
	private:
		uint32_t rendererId;
		std::string m_Name;

		std::unordered_map<std::string, GLint> uniformLocationCache{};
	};

}