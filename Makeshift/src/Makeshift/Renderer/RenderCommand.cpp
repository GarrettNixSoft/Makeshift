#include "mkpch.hpp"
#include "RenderCommand.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Makeshift {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}