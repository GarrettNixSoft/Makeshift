#include "mkpch.hpp"
#include "VulkanShader.hpp"

#include "VulkanContext.hpp"

#include "Makeshift/Core/Timer.hpp"

#include <shaderc/shaderc.hpp>

#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <filesystem>

namespace Makeshift {

	namespace Utils {

		static VulkanShaderStage shaderTypeFromString(const std::string& type) {
			if (type == "vertex")
				return VulkanShaderStage::VERTEX;
			else if (type == "fragment" || type == "pixel")
				return VulkanShaderStage::FRAGMENT;

			MK_CORE_ASSERT(false, "Unknown shader type!");
			return VulkanShaderStage::UNKNOWN;
		}

		static shaderc_shader_kind VulkanShaderStageToShaderC(VulkanShaderStage stage) {
			switch (stage) {
				case VulkanShaderStage::VERTEX:		return shaderc_glsl_vertex_shader;
				case VulkanShaderStage::FRAGMENT:	return shaderc_glsl_fragment_shader;
			}
			MK_CORE_ASSERT(false, "Unsupported shader stage");
			return (shaderc_shader_kind)0;
		}

		static const char* VulkanShaderStageToString(VulkanShaderStage stage) {
			switch (stage) {
				case VulkanShaderStage::VERTEX:		return "VERTEX_SHADER";
				case VulkanShaderStage::FRAGMENT:	return "FRAGMENT_SHADER";
			}
			MK_CORE_ASSERT(false, "Unsupported shader stage");
			return nullptr;
		}

		static const char* GetCacheDirectory() {
			// TODO: validate directory path
			return "assets/cache/shader/vulkan";
		}

		static void CreateCacheDirectoryIfNeeded() {
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* VulkanShaderStageCachedVulkanFileExtension(VulkanShaderStage stage) {
			switch (stage) {
				case VulkanShaderStage::VERTEX:		return ".cached_vulkan.vert";
				case VulkanShaderStage::FRAGMENT:	return ".cached_vulkan.frag";
			}
			MK_CORE_ASSERT(false, "Unsupported shader stage");
			return "";
		}

	}

	VulkanShader::VulkanShader(const std::string& filepath) : m_FilePath(filepath) {
		MK_PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = readFile(filepath);
		auto shaderSources = preprocess(source);

		{
			Timer timer;
			compileOrGetVulkanBinaries(shaderSources);
			createProgram();
			MK_CORE_WARN("Shader creation took {0}ms", timer.elapsedMillis());
		}

		// Rip name out of the filePath. Example: assets/shaders/name.ext --> name
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	VulkanShader::VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name) {
		MK_PROFILE_FUNCTION();

		std::unordered_map<VulkanShaderStage, std::string> sources;
		sources[VulkanShaderStage::VERTEX] = vertexSrc;
		sources[VulkanShaderStage::FRAGMENT] = fragmentSrc;

		compileOrGetVulkanBinaries(sources);
		createProgram();
	}

	VulkanShader::~VulkanShader() {
		vkDestroyShaderModule(VulkanContext::GetDevice(), m_VertexModule, nullptr);
		vkDestroyShaderModule(VulkanContext::GetDevice(), m_FragmentModule, nullptr);
	}

	void VulkanShader::bind() const {

	}

	void VulkanShader::unbind() const {

	}

	std::string VulkanShader::readFile(const std::string& filepath) {
		MK_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			MK_CORE_ERROR("Could not load shader file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<VulkanShaderStage, std::string> VulkanShader::preprocess(const std::string& source) {
		MK_PROFILE_FUNCTION();

		std::unordered_map<VulkanShaderStage, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			MK_CORE_ASSERT(eol != std::string::npos, "Syntax error (EOL)");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			MK_CORE_ASSERT(Utils::shaderTypeFromString(type) != VulkanShaderStage::UNKNOWN, "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[Utils::shaderTypeFromString(type)] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;

	}

	void VulkanShader::compileOrGetVulkanBinaries(const std::unordered_map<VulkanShaderStage, std::string>& shaderSources) {

		//GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		//options.AddMacroDefinition("VULKAN"); // Example for using preprocessor to compile different code in the shaders for Vulkan/OpenGL builds
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources) {

			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::VulkanShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open()) { // READ FROM CACHE
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else { // COMPILE NEW RESULT

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::VulkanShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					MK_CORE_ERROR(module.GetErrorMessage());
					MK_CORE_ASSERT(false, "Shader compilation failed!");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData) {
			reflect(stage, data);
		}

	}

	void VulkanShader::createProgram() {
		MK_PROFILE_FUNCTION();

		{	// CREATE VERTEX SHADER MODULE
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = m_VulkanSPIRV[VulkanShaderStage::VERTEX].size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(m_VulkanSPIRV[VulkanShaderStage::VERTEX].data());

			if (vkCreateShaderModule(VulkanContext::GetDevice(), &createInfo, nullptr, &m_VertexModule) != VK_SUCCESS) {
				MK_CORE_ASSERT(false, "Failed to create vertex shader module for {0}", m_FilePath);
			}
		}

		{	// CREATE FRAGMENT SHADER MODULE
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = m_VulkanSPIRV[VulkanShaderStage::FRAGMENT].size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(m_VulkanSPIRV[VulkanShaderStage::FRAGMENT].data());

			if (vkCreateShaderModule(VulkanContext::GetDevice(), &createInfo, nullptr, &m_VertexModule) != VK_SUCCESS) {
				MK_CORE_ASSERT(false, "Failed to create vertex shader module for {0}", m_FilePath);
			}
		}

	}

	void VulkanShader::reflect(VulkanShaderStage stage, const std::vector<uint32_t>& shaderData) {

		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		MK_CORE_TRACE("VulkanShader::reflect - {0} {1}", Utils::VulkanShaderStageToString(stage), m_FilePath);
		MK_CORE_TRACE("\t{0} uniform buffers", resources.uniform_buffers.size());
		MK_CORE_TRACE("\t{0} resources", resources.sampled_images.size());

		MK_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers) {
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			MK_CORE_TRACE("\t{0}", resource.name);
			MK_CORE_TRACE("\t\tSize = {0}", bufferSize);
			MK_CORE_TRACE("\t\tBinding = {0}", binding);
			MK_CORE_TRACE("\t\tMembers = {0}", memberCount);
		}

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
