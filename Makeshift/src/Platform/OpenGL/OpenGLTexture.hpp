#pragma once

#include "Makeshift/Renderer/Texture.hpp"

namespace Makeshift {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t getWidth() const override { return width; }
		virtual uint32_t getHeight() const override { return height; }

		virtual void bind(uint32_t slot = 0) const override;
	private:
		std::string path;
		uint32_t width;
		uint32_t height;
		uint32_t rendererId;
	};

}