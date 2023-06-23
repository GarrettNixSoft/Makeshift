#include "mkpch.hpp"
#include "VulkanShader.hpp"

namespace Makeshift {

	VulkanShader::VulkanShader(const std::string& filePath) {

	}

	VulkanShader::VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name) {

	}

	VulkanShader::~VulkanShader() {

	}

	void VulkanShader::bind() const {

	}

	void VulkanShader::unbind() const {

	}

	void VulkanShader::setFloat(const std::string& name, float value) {
		
	}

	void VulkanShader::setInt(const std::string& name, int value) {
		
	}

	void VulkanShader::setBool(const std::string& name, bool value) {
		
	}

	void VulkanShader::setIntArray(const std::string& name, int* values, uint32_t count) {

	}

	void VulkanShader::setVec2(const std::string& name, const glm::vec2& value) {
		
	}

	void VulkanShader::setVec3(const std::string& name, const glm::vec3& value) {
		
	}

	void VulkanShader::setVec4(const std::string& name, const glm::vec4& value) {
		
	}

	void VulkanShader::setMat3(const std::string& name, const glm::mat3& value) {
		
	}

	void VulkanShader::setMat4(const std::string& name, const glm::mat4& value) {
		
	}

}