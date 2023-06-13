#pragma once

#include "Makeshift/Renderer/Shader.hpp"
#include <glm/glm.hpp>

#include <glad/glad.h>

namespace Makeshift {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

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
		inline GLint getUniformLocation(const std::string& name) {
			GLint location;
			if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
				location = uniformLocationCache[name];
			}
			else {
				location = glGetUniformLocation(rendererId, name.c_str());
				uniformLocationCache.emplace(name, location);
				//MK_CORE_TRACE("Cached location for uniform: {0}", name);
			}
			return location;
		}
	private:
		uint32_t rendererId;

		std::unordered_map<std::string, GLint> uniformLocationCache{};
	};

}