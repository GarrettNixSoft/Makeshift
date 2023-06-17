#include "mkpch.hpp"
#include "OpenGLTexture.hpp"

#include "stb_image.h"

#include <glad/glad.h>

namespace Makeshift {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath) : path(filePath) {

		int w, h, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
		MK_CORE_ASSERT(data, "Failed to load image!");

		width = w;
		height = h;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		MK_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
		glTextureStorage2D(rendererId, 1, internalFormat, width, height);

		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(rendererId, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &rendererId);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const {
		glBindTextureUnit(slot, rendererId);
	}

}