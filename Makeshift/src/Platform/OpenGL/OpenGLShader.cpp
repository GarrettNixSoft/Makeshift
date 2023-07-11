#include "mkpch.hpp"
#include "OpenGLShader.hpp"

#include "Makeshift/Core/Timer.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>

#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <fstream>
#include <filesystem>


namespace Makeshift {

	namespace Utils {

		static GLenum shaderTypeFromString(const std::string& type) {
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			else if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			MK_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
			switch (stage) {
				case GL_VERTEX_SHADER:		return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER:	return shaderc_glsl_fragment_shader;
			}
			MK_CORE_ASSERT(false, "Unsupported shader stage");
			return (shaderc_shader_kind)0;
		}

		static const char* GLShaderStageToString(GLenum stage) {
			switch (stage) {
				case GL_VERTEX_SHADER:		return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER:	return "GL_FRAGMENT_SHADER";
			}
			MK_CORE_ASSERT(false, "Unsupported shader stage");
			return nullptr;
		}

		static const char* GetCacheDirectory() {
			// TODO: validate directory path
			return "assets/cache/shader/opengl";
		}

		static void CreateCacheDirectoryIfNeeded() {
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage) {
			switch (stage) {
				case GL_VERTEX_SHADER:		return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:	return ".cached_opengl.frag";
			}
			MK_CORE_ASSERT(false, "Unsupported shader stage");
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage) {
			switch (stage) {
				case GL_VERTEX_SHADER:		return ".cached_vulkan.vert";
				case GL_FRAGMENT_SHADER:	return ".cached_vulkan.frag";
			}
			MK_CORE_ASSERT(false, "Unsupported shader stage");
			return "";
		}
	}

		GLint OpenGLShader::getUniformLocation(const std::string& name) {
			GLint location;
			if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
				location = uniformLocationCache[name];
			}
			else {
				location = glGetUniformLocation(m_RendererId, name.c_str());
				uniformLocationCache.emplace(name, location);
				//MK_CORE_TRACE("Cached location for uniform: {0}", name);
			}
			return location;
		}

	OpenGLShader::OpenGLShader(const std::string& filepath) : m_FilePath(filepath) {
		MK_PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = readFile(filepath);
		auto shaderSources = preprocess(source);

		{
			Timer timer;
			compileOrGetVulkanBinaries(shaderSources);
			compileOrGetOpenGLBinaries();
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

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name) {
		MK_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		compileOrGetVulkanBinaries(sources);
		compileOrGetOpenGLBinaries();
		createProgram();
	}

	OpenGLShader::~OpenGLShader() {
		MK_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererId);
	}

	std::string OpenGLShader::readFile(const std::string& filePath) {
		MK_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			MK_CORE_ERROR("Could not load shader file '{0}'", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preprocess(const std::string& source) {
		MK_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			MK_CORE_ASSERT(eol != std::string::npos, "Syntax error (EOL)");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			MK_CORE_ASSERT(Utils::shaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[Utils::shaderTypeFromString(type)] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;

	}
	
	void OpenGLShader::compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources) {

		//GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		//options.AddMacroDefinition("OPENGL"); // Example for using preprocessor to compile different code in the shaders for Vulkan/OpenGL builds
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources) {

			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

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

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
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

	void OpenGLShader::compileOrGetOpenGLBinaries() {

		// Compile (TODO: Read cache if available)
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV) {

			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else {
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					MK_CORE_ERROR(module.GetErrorMessage());
					MK_CORE_ASSERT(false, "Shader compilation failed!");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*) data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::createProgram() {
		MK_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		
		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV) {
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {

			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			MK_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);

		}
		
		for (auto id : shaderIDs) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererId = program;
	}

	void OpenGLShader::reflect(GLenum stage, const std::vector<uint32_t>& shaderData) {

		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		MK_CORE_TRACE("OpenGLShader::reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
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

	void OpenGLShader::bind() const {
		MK_PROFILE_FUNCTION();

		glUseProgram(m_RendererId);
	}

	void OpenGLShader::unbind() const {
		MK_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::setFloat(const std::string& name, float value) {
		MK_PROFILE_FUNCTION();

		uploadUniformFloat(name, value);
	}

	void OpenGLShader::setInt(const std::string& name, int value) {
		MK_PROFILE_FUNCTION();

		uploadUniformInt(name, value);
	}

	void OpenGLShader::setBool(const std::string& name, bool value) {
		MK_PROFILE_FUNCTION();

		uploadUniformBool(name, value);
	}

	void OpenGLShader::setIntArray(const std::string& name, int* values, uint32_t count) {
		MK_PROFILE_FUNCTION();

		uploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::setVec2(const std::string& name, const glm::vec2& value) {
		MK_PROFILE_FUNCTION();

		uploadUniformVec2(name, value);
	}

	void OpenGLShader::setVec3(const std::string& name, const glm::vec3& value) {
		MK_PROFILE_FUNCTION();

		uploadUniformVec3(name, value);
	}

	void OpenGLShader::setVec4(const std::string& name, const glm::vec4& value) {
		MK_PROFILE_FUNCTION();

		uploadUniformVec4(name, value);
	}

	void OpenGLShader::setMat3(const std::string& name, const glm::mat3& value) {
		MK_PROFILE_FUNCTION();

		uploadUniformMat3(name, value);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value) {
		MK_PROFILE_FUNCTION();

		uploadUniformMat4(name, value);
	}

	void OpenGLShader::uploadUniformFloat(const std::string& name, float value) {
		MK_PROFILE_FUNCTION();

		GLint location = getUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniformVec2(const std::string& name, const glm::vec2& vector) {
		GLint location = getUniformLocation(name);
		glUniform2f(location, vector.x, vector.y);
	}

	void OpenGLShader::uploadUniformVec3(const std::string& name, const glm::vec3& vector) {
		GLint location = getUniformLocation(name);
		glUniform3f(location, vector.x, vector.y, vector.z);
	}


	void OpenGLShader::uploadUniformVec4(const std::string& name, const glm::vec4& vector) {
		GLint location = getUniformLocation(name);
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, int value) {
		GLint location = getUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniformVec2i(const std::string& name, const glm::ivec2& vector) {
		GLint location = getUniformLocation(name);
		glUniform2i(location, vector.x, vector.y);
	}

	void OpenGLShader::uploadUniformVec3i(const std::string& name, const glm::ivec4& vector) {
		GLint location = getUniformLocation(name);
		glUniform3i(location, vector.x, vector.y, vector.z);
	}

	void OpenGLShader::uploadUniformVec4i(const std::string& name, const glm::ivec4& vector) {
		GLint location = getUniformLocation(name);
		glUniform4i(location, vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::uploadUniformIntArray(const std::string& name, int* values, uint32_t count) {
		GLint location = getUniformLocation(name);
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
		GLint location = getUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
		GLint location = getUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformBool(const std::string& name, bool value) {
		GLint location = getUniformLocation(name);
		glUniform1i(location, value);
	}

}