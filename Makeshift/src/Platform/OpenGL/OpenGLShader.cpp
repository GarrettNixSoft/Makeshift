#include "mkpch.hpp"
#include "OpenGLShader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Makeshift {

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
		// Read our shaders into the appropriate buffers
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			MK_CORE_ERROR("{0}", infoLog.data());
			MK_CORE_ASSERT(false, "Vertex shader compile failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			MK_CORE_ERROR("{0}", infoLog.data());
			MK_CORE_ASSERT(false, "Fragment shader compile failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		rendererId = glCreateProgram();
		GLuint program = rendererId;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

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
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			MK_CORE_ERROR("{0}", infoLog.data());
			MK_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(rendererId);
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