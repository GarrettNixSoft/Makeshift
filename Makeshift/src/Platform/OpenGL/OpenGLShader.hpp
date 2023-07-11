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

		virtual void setFloat(const std::string& name, float value) override;
		virtual void setInt(const std::string& name, int value) override;
		virtual void setBool(const std::string& name, bool value) override;

		virtual void setIntArray(const std::string& name, int* values, uint32_t count) override;

		virtual void setVec2(const std::string& name, const glm::vec2& value) override;
		virtual void setVec3(const std::string& name, const glm::vec3& value) override;
		virtual void setVec4(const std::string& name, const glm::vec4& value) override;

		virtual void setMat3(const std::string& name, const glm::mat3& value) override;
		virtual void setMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string& getName() const override { return m_Name; };

		void uploadUniformFloat(const std::string& name, float value);
		void uploadUniformVec2(const std::string& name, const glm::vec2& vector);
		void uploadUniformVec3(const std::string& name, const glm::vec3& vector);
		void uploadUniformVec4(const std::string& name, const glm::vec4& vector);

		void uploadUniformInt(const std::string& name, int value);
		void uploadUniformVec2i(const std::string& name, const glm::ivec2& vector);
		void uploadUniformVec3i(const std::string& name, const glm::ivec4& vector);
		void uploadUniformVec4i(const std::string& name, const glm::ivec4& vector);

		void uploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		void uploadUniformBool(const std::string& name, bool value);
	private:
		std::string readFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> preprocess(const std::string& source);

		void compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
		void compileOrGetOpenGLBinaries();

		void createProgram();

		void reflect(GLenum stage, const std::vector<uint32_t>& shaderData);
	private:
		GLint getUniformLocation(const std::string& name);
	private:
		uint32_t m_RendererId;
		std::string m_FilePath;
		std::string m_Name;

		std::unordered_map<std::string, GLint> uniformLocationCache{};

		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
	};

}