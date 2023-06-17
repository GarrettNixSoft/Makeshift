#pragma once

#include "OrthographicCamera.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>

namespace Makeshift {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Render Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float roation = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float tiling = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color = glm::vec4(1.0f), float rotation = 0.0f, float tiling = 1.0f);

	};

}