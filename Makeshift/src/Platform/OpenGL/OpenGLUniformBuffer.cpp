#include "mkpch.hpp"
#include "OpenGLUniformBuffer.hpp"

#include <glad/glad.h>

namespace Makeshift {

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
		glCreateBuffers(1, &m_RendererId);
		glNamedBufferData(m_RendererId, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererId);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset) {
		glNamedBufferSubData(m_RendererId, offset, size, data);
	}

}
