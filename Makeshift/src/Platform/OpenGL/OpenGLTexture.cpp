#include "mkpch.hpp"
#include "OpenGLTexture.hpp"

#include "stb_image.h"

#include <glad/glad.h>

namespace Makeshift {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {
		
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
		glTextureStorage2D(rendererId, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath) : path(filePath) {

		int w, h, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
		MK_CORE_ASSERT(data, "Failed to load image!");

		m_Width = w;
		m_Height = h;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		MK_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
		glTextureStorage2D(rendererId, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(rendererId, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &rendererId);
	}

	void OpenGLTexture2D::setData(void* data, uint32_t size) {

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;

		MK_CORE_ASSERT(size == m_Width * m_Height * bpp, "Size of data provided must fill entire texture");
		glTextureSubImage2D(rendererId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

	}

	void OpenGLTexture2D::bind(uint32_t slot) const {
		glBindTextureUnit(slot, rendererId);
	}

}