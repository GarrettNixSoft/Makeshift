#include "mkpch.hpp"
#include "OpenGLVertexArray.hpp"

#include <glad/glad.h>

namespace Makeshift {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {

		switch (type) {
			case ShaderDataType::Float:	return GL_FLOAT;
			case ShaderDataType::Int:	return GL_INT;
			case ShaderDataType::Bool:	return GL_BOOL;
			case ShaderDataType::Vec2:	return GL_FLOAT;
			case ShaderDataType::Vec3:	return GL_FLOAT;
			case ShaderDataType::Vec4:	return GL_FLOAT;
			case ShaderDataType::Mat3:	return GL_FLOAT;
			case ShaderDataType::Mat4:	return GL_FLOAT;
			case ShaderDataType::Vec2i:	return GL_INT;
			case ShaderDataType::Vec3i:	return GL_INT;
			case ShaderDataType::Vec4i:	return GL_INT;
		}

		MK_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &rendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &rendererId);
	}

	void OpenGLVertexArray::bind() const {
		glBindVertexArray(rendererId);
	}

	void OpenGLVertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		MK_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(rendererId);
		vertexBuffer->bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout) {

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.offset);
			index++;

		}

		vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(rendererId);
		indexBuffer->bind();

		this->indexBuffer = indexBuffer;
	}

}
