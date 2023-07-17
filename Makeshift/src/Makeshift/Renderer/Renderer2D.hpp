#pragma once

#include "Makeshift/Renderer/Camera.hpp"
#include "Makeshift/Renderer/OrthographicCamera.hpp"
#include "Makeshift/Renderer/EditorCamera.hpp"

#include "Makeshift/Renderer/Texture.hpp"
#include "Makeshift/Renderer/SubTexture2D.hpp"

#include "Makeshift/Scene/Components.hpp"

#include <glm/glm.hpp>

namespace Makeshift {

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera); // TODO remove
		static void EndScene();

		static void StartBatch();
		static void Flush();

		// Render Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f, int entityId = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f, int entityId = -1);

		// Rotation should be specified in radians
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor = glm::vec4(1.0f), float tiling = 1.0f);

		static void DrawTriangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
		static void DrawTriangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);

		// Render Objects
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityId);
		static void DrawSprite(const glm::mat4& transform, SpriteSubtextureRendererComponent& src, int entityId);

		// Render stats
		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:
		static void NextBatch();
	};

}
