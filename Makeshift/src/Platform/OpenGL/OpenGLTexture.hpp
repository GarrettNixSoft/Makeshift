#pragma once

#include "Makeshift/Renderer/Texture.hpp"

#include <glad/glad.h>

namespace Makeshift {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t getWidth() const override { return m_Width; }
		virtual uint32_t getHeight() const override { return m_Height; }
		virtual uint32_t getRendererId() const override { return m_RendererId; }

		virtual void setData(void* data, uint32_t size) override;

		virtual void bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override { return m_RendererId == ((OpenGLTexture2D&)other).m_RendererId; };
	private:
		std::string path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererId;
		GLenum m_InternalFormat, m_DataFormat;
	};

}