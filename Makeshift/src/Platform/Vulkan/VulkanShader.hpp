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

		virtual void setFloat(const std::string& name, float value) override;
		virtual void setInt(const std::string& name, int value) override;
		virtual void setBool(const std::string& name, int value) override;

		virtual void setVec2(const std::string& name, const glm::vec2& value) override;
		virtual void setVec3(const std::string& name, const glm::vec3& value) override;
		virtual void setVec4(const std::string& name, const glm::vec4& value) override;

		virtual void setMat3(const std::string& name, const glm::mat3& value) override;
		virtual void setMat4(const std::string& name, const glm::mat4& value) override;
	private:
		uint32_t rendererId;
		std::string m_Name;
	};

}