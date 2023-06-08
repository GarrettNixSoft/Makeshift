#pragma once

namespace Makeshift {

	enum class RendererAPI {
		None = 0,
		OpenGL = 1,
		Vulkan = 2
	};

	class Renderer {
	public:
		static inline RendererAPI GetAPI() { return rendererAPI; }

	private:
		static RendererAPI rendererAPI;

	};


}