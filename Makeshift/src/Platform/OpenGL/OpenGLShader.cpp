#include "mkpch.hpp"
#include "OpenGLShader.hpp"

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Makeshift {

	static GLenum shaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		MK_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	GLint OpenGLShader::getUniformLocation(const std::string& name) {
		GLint location;
		if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
			location = uniformLocationCache[name];
		}
		else {
			location = glGetUniformLocation(rendererId, name.c_str());
			uniformLocationCache.emplace(name, location);
			//MK_CORE_TRACE("Cached location for uniform: {0}", name);
		}
		return location;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath) {
		std::string source = readFile(filePath);
		auto shaderSources = preprocess(source);
		compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(rendererId);
	}

	std::string OpenGLShader::readFile(const std::string& filePath) {
		std::string result;
		std::ifstream in(filePath, std::ios::in, std::ios::binary);
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

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			MK_CORE_ASSERT(eol != std::string::npos, "Syntax error (EOL)");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			MK_CORE_ASSERT(shaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[shaderTypeFromString(type)] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;

	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources) {

		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shaderSources.size());

		for (auto& kv : shaderSources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			
			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				MK_CORE_ERROR("{0}", infoLog.data());
				MK_CORE_ASSERT(false, "Shader compile failure!");
				return;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.
			MK_CORE_ERROR("{0}", infoLog.data());
			MK_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs) {
			glDetachShader(program, id);
		}

		rendererId = program;
	}

	void OpenGLShader::bind() const {
		glUseProgram(rendererId);
	}

	void OpenGLShader::unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::uploadUniformFloat(const std::string& name, float value) {
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

	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
		GLint location = getUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
		GLint location = getUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}