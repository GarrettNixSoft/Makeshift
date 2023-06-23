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

		static void Flush();

		// Render Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);

		static void DrawTriangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
		static void DrawTriangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);

	};

}