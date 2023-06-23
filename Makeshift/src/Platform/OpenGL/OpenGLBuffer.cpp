#include "mkpch.hpp"
#include "OpenGLBuffer.hpp"

#include <glad/glad.h>

namespace Makeshift {

	// ================================ VERTEX BUFFERS ================================

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
		MK_PROFILE_FUNCTION();

		glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		MK_PROFILE_FUNCTION();

		glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		MK_PROFILE_FUNCTION();

		glDeleteBuffers(1, &rendererId);
	}

	void OpenGLVertexBuffer::bind() const {
		MK_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
	}

	void OpenGLVertexBuffer::unbind() const {
		MK_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::setData(const void* data, uint32_t size) {

		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	}

	// ================================ INDEX BUFFERS ================================

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : count{ count } {
		MK_PROFILE_FUNCTION();

        glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		MK_PROFILE_FUNCTION();

        glDeleteBuffers(1, &rendererId);
    }

    void OpenGLIndexBuffer::bind() const {
		MK_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
    }

    void OpenGLIndexBuffer::unbind() const {
		MK_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}