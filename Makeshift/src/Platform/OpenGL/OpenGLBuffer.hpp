#pragma once

#include "Makeshift/Renderer/Buffer.hpp"

namespace Makeshift {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setData(const void* data, uint32_t size) override;

		virtual const BufferLayout& getLayout() const { return layout; };
		virtual void setLayout(const BufferLayout& newLayout) { layout = newLayout; };
	private:
		uint32_t rendererId;
		BufferLayout layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual uint32_t getCount() const override { return count; }

		virtual void bind() const override;
		virtual void unbind() const override;
	private:
		uint32_t rendererId;
		uint32_t count;
	};

}