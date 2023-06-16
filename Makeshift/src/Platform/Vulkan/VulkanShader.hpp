#pragma once

#include "Makeshift/Renderer/Shader.hpp"

#include <glm/glm.hpp>

#include <string>

namespace Makeshift {

	class VulkanShader : public Shader {
	public:
		VulkanShader(const std::string& filePath);
		VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~VulkanShader();

		virtual const std::string& getName() const override { return m_Name; };

		virtual void bind() const override;
		virtual void unbind() const override;
	private:
		uint32_t rendererId;
		std::string m_Name;
	};

}