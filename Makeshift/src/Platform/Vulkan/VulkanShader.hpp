#pragma once

#include "Makeshift/Renderer/Shader.hpp"

#include "vulkan/vulkan.hpp"

#include <glm/glm.hpp>

#include <string>

namespace Makeshift {

	enum class VulkanShaderStage : uint32_t {
		UNKNOWN = 0, VERTEX = 1, FRAGMENT = 2
	};

	class VulkanShader : public Shader {
	public:
		VulkanShader(const std::string& filepath);
		VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~VulkanShader();

		virtual const std::string& getName() const override { return m_Name; };

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setFloat(const std::string& name, float value) override;
		virtual void setInt(const std::string& name, int value) override;
		virtual void setBool(const std::string& name, bool value) override;

		virtual void setIntArray(const std::string& name, int* values, uint32_t count);

		virtual void setVec2(const std::string& name, const glm::vec2& value) override;
		virtual void setVec3(const std::string& name, const glm::vec3& value) override;
		virtual void setVec4(const std::string& name, const glm::vec4& value) override;

		virtual void setMat3(const std::string& name, const glm::mat3& value) override;
		virtual void setMat4(const std::string& name, const glm::mat4& value) override;
	private:
		std::string readFile(const std::string& filepath);
		std::unordered_map<VulkanShaderStage, std::string> preprocess(const std::string& source);

		void compileOrGetVulkanBinaries(const std::unordered_map<VulkanShaderStage, std::string>& shaderSources);

		void createProgram();

		void reflect(VulkanShaderStage stage, const std::vector<uint32_t>& shaderData);
	private:
		VkDevice parentDevice;

		VkShaderModule m_VertexModule;
		VkShaderModule m_FragmentModule;
		std::string m_FilePath;
		std::string m_Name;

		std::unordered_map<VulkanShaderStage, std::vector<uint32_t>> m_VulkanSPIRV;
	};

}
