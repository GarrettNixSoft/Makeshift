#pragma once

#include "Makeshift/Renderer/VertexArray.hpp"

namespace Makeshift {

	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override { return vertexBuffers; };
		virtual const Ref<IndexBuffer>& getIndexBuffer() const override { return indexBuffer;  };
	private:
		uint32_t rendererId;
		std::vector<Ref<VertexBuffer>> vertexBuffers;
		Ref<IndexBuffer> indexBuffer;
	};

}
