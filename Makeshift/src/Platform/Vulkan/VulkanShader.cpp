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

}