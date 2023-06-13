#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Makeshift {

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
		virtual void uploadUniformVec4(const std::string& name, const glm::vec4& vector) = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	private:
		uint32_t rendererId;
	};

}