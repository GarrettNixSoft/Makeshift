#pragma once

// For use by Makeshift applications

// ---------------- Core --------------------------------------
#include "Makeshift/Core/Application.hpp"
#include "Makeshift/Core/Layer.hpp"
#include "Makeshift/Core/Logger.hpp"

#include "Makeshift/Core/Timestep.hpp"

#include "Makeshift/Core/Input.hpp"
#include "Makeshift/Core/Keycodes.hpp"
#include "Makeshift/Core/MouseCodes.hpp"
// ------------------------------------------------------------

// ---------------- ImGui -------------------------------------
#include "Makeshift/ImGui/ImGuiLayer.hpp"
// ------------------------------------------------------------

// ---------------- Scene -------------------------------------
#include "Makeshift/Scene/Scene.hpp"
#include "Makeshift/Scene/Entity.hpp"
#include "Makeshift/Scene/ScriptableEntity.hpp"
#include "Makeshift/Scene/Components.hpp"
// ------------------------------------------------------------

// ---------------- Renderer ----------------------------------
#include "Makeshift/Renderer/Renderer.hpp"
#include "Makeshift/Renderer/Renderer2D.hpp"
#include "Makeshift/Renderer/RenderCommand.hpp"
#include "Makeshift/Renderer/RendererAPI.hpp"

#include "Makeshift/Renderer/Buffer.hpp"
#include "Makeshift/Renderer/Shader.hpp"
#include "Makeshift/Renderer/Framebuffer.hpp"
#include "Makeshift/Renderer/Texture.hpp"
#include "Makeshift/Renderer/SubTexture2D.hpp"
#include "Makeshift/Renderer/VertexArray.hpp"

#include "Makeshift/Renderer/OrthographicCameraController.hpp"
// ------------------------------------------------------------