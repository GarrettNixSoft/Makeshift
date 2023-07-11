#pragma once

#include "Makeshift/Renderer/UniformBuffer.hpp"

namespace Makeshift {

	class OpenGLUniformBuffer : public UniformBuffer {
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t m_RendererId = 0;
	};

}
