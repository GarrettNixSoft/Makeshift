#pragma once

#include "Makeshift/Renderer/Shader.hpp"

#include <glm/glm.hpp>

namespace Makeshift {

	class VulkanShader : public Shader {
	public:
		VulkanShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~VulkanShader();

		virtual void bind() const override;
		virtual void unbind() const override;
	private:
		uint32_t rendererId;
	};

}