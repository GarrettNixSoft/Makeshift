#pragma once

#include "Makeshift/Renderer/Buffer.hpp"

namespace Makeshift {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;
	private:
		uint32_t rendererId;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual uint32_t getCount() const { return count; }

		virtual void bind() const override;
		virtual void unbind() const override;
	private:
		uint32_t rendererId;
		uint32_t count;
	};

}