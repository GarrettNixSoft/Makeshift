#pragma once

#include "Makeshift/Renderer/Shader.hpp"

namespace Makeshift {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;
	private:
		uint32_t rendererId;
	};

}