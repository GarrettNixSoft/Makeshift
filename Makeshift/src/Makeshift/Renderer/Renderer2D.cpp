#include "mkpch.hpp"
#include "Renderer2D.hpp"

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "UniformBuffer.hpp"

#include "RenderCommand.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Makeshift {

	struct QuadVertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;
		// Workshop-only
		int entityId = -1;
	};

	struct TriangleVertex {
		glm::vec3 position;
		glm::vec2 texCoord;
	};

	struct Renderer2DData {

		static const uint32_t MAX_QUADS = 20000;
		static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
		static const uint32_t MAX_INDICES = MAX_QUADS * 6;

		static const uint32_t MAX_TRIANGLES = 10000;
		static const uint32_t MAX_TRI_VERTICES = MAX_TRIANGLES * 3;
		static const uint32_t MAX_TRI_INDICES = MAX_TRIANGLES * 3;

		static const uint32_t MAX_TEXTURE_SLOTS = 32; // TODO RenderCapabilities

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;

		Ref<VertexArray> triangleVertexArray;

		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture; // slot 0

		uint32_t quadIndexCount = 0;
		uint32_t triangleIndexCount = 0;

		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		TriangleVertex* triangleVertexBufferBase = nullptr;
		TriangleVertex* triangleVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = white texture

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;

		struct CameraData {
			glm::mat4 viewProjection;
		};
		CameraData cameraBuffer;
		Ref<UniformBuffer> cameraUniformBuffer;

	};

	static Renderer2DData s_Data;

	void Renderer2D::Init() {
		MK_PROFILE_FUNCTION();

		// ================================ QUADS ================================
		s_Data.quadVertexArray = VertexArray::Create();

		s_Data.quadVertexBuffer = VertexBuffer::Create(s_Data.MAX_VERTICES * sizeof(QuadVertex));

		s_Data.quadVertexBuffer->setLayout({
			{ ShaderDataType::Vec3,		"position" },
			{ ShaderDataType::Vec4,		"color" },
			{ ShaderDataType::Vec2,		"texCoord" },
			{ ShaderDataType::Float,	"texIndex" },
			{ ShaderDataType::Float,	"tilingFactor" },
			{ ShaderDataType::Int,		"entityId" }
		});
		s_Data.quadVertexArray->addVertexBuffer(s_Data.quadVertexBuffer);

		s_Data.quadVertexBufferBase = new QuadVertex[s_Data.MAX_VERTICES];

		uint32_t* quadIndices = new uint32_t[s_Data.MAX_INDICES];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MAX_INDICES; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MAX_INDICES);
		s_Data.quadVertexArray->setIndexBuffer(quadIB);
		delete[] quadIndices;
		// ================================ QUADS ================================

		// ================================ TRIANGLES ================================
		s_Data.triangleVertexArray = VertexArray::Create();

		Ref<VertexBuffer> triangleVB = VertexBuffer::Create(s_Data.MAX_TRIANGLES * sizeof(TriangleVertex));

		triangleVB->setLayout({
			{ ShaderDataType::Vec3, "position" },
			{ ShaderDataType::Vec4, "color" },
			{ ShaderDataType::Vec2, "texCoord" }
			});
		s_Data.triangleVertexArray->addVertexBuffer(triangleVB);

		uint32_t* triangleIndices = new uint32_t[s_Data.MAX_TRI_INDICES];
		Ref<IndexBuffer> triangleIB = IndexBuffer::Create(triangleIndices, s_Data.MAX_TRI_INDICES);
		s_Data.triangleVertexArray->setIndexBuffer(triangleIB);
		delete[] triangleIndices;
		// ================================ TRIANGLES ================================

		// ================================ TEXTURES ================================
		s_Data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MAX_TEXTURE_SLOTS];
		for (uint32_t i = 0; i < s_Data.MAX_TEXTURE_SLOTS; i++) {
			samplers[i] = i;
		}
		// ================================ TEXTURES ================================

		// ================================ SHADERS ================================
		s_Data.textureShader = Shader::Create("assets/shaders/texture.glsl");

		// set the first texture slot to 0
		s_Data.textureSlots[0] = s_Data.whiteTexture;
		// ================================ SHADERS ================================

		s_Data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		s_Data.quadVertexArray->bind();

		s_Data.cameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);

	}

	void Renderer2D::Shutdown() {
		MK_PROFILE_FUNCTION();
		
		delete[] s_Data.quadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		MK_PROFILE_FUNCTION();

		s_Data.textureShader->bind();
		s_Data.textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
		MK_PROFILE_FUNCTION();

		s_Data.cameraBuffer.viewProjection = camera.getProjection() * glm::inverse(transform);
		s_Data.cameraUniformBuffer->setData(&s_Data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera) {
		MK_PROFILE_FUNCTION();

		s_Data.cameraBuffer.viewProjection = camera.getViewProjection();
		s_Data.cameraUniformBuffer->setData(&s_Data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene() {
		MK_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch() {
		// reset to base of the vertex buffer array
		s_Data.quadIndexCount = 0;
		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;

		s_Data.textureSlotIndex = 1;
	}

	void Renderer2D::Flush() {
		MK_PROFILE_FUNCTION();

		// Check if there's anything to draw at all
		if (s_Data.quadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase);
		s_Data.quadVertexBuffer->setData(s_Data.quadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++) {
			s_Data.textureSlots[i]->bind(i);
		}

		s_Data.textureShader->bind();
		RenderCommand::DrawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
		s_Data.stats.DrawCalls++;

	}

	void Renderer2D::NextBatch() {
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {

		DrawQuad({ position.x, position.y, 0.0f }, size, color);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		MK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tintColor, float tiling) {

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, { 1.0f,1.0f,1.0f,1.0f }, tiling);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4 & tintColor, float tiling) {
		MK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tintColor, tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor, float tiling) {

		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, { 1.0f,1.0f,1.0f,1.0f }, tiling);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor, float tiling) {
		MK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subtexture, tintColor, tiling);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId) {

		MK_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const float textureIndex = 0.0f; // white texture
		const float tilingFactor = 1.0f;

		if (s_Data.quadIndexCount >= Renderer2DData::MAX_INDICES) {
			NextBatch();
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = color;
			s_Data.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_Data.quadVertexBufferPtr->entityId = entityId;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;

		s_Data.stats.QuadCount++;

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D> texture, const glm::vec4& tintColor, float tiling, int entityId) {

		MK_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.quadIndexCount >= Renderer2DData::MAX_INDICES) {
			NextBatch();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++) {
			if (*s_Data.textureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_Data.textureSlotIndex >= Renderer2DData::MAX_TEXTURE_SLOTS)
				NextBatch();
			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = color;
			s_Data.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->tilingFactor = tiling;
			s_Data.quadVertexBufferPtr->entityId = entityId;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;

		s_Data.stats.QuadCount++;

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor, float tiling, int entityId) {
		MK_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subtexture->getTexCoords();
		const Ref<Texture2D> texture = subtexture->getTexture();

		if (s_Data.quadIndexCount >= Renderer2DData::MAX_INDICES) {
			NextBatch();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++) {
			if (*s_Data.textureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
			s_Data.quadVertexBufferPtr->color = color;
			s_Data.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_Data.quadVertexBufferPtr->texIndex = textureIndex;
			s_Data.quadVertexBufferPtr->tilingFactor = tiling;
			s_Data.quadVertexBufferPtr->entityId = entityId;
			s_Data.quadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;

		s_Data.stats.QuadCount++;

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		MK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& tintColor, float tiling) {

		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tintColor, tiling);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const glm::vec4& tintColor, float tiling) {
		MK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tintColor, tiling);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor, float tiling) {

		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, tintColor, tiling);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D> subtexture, const glm::vec4& tintColor, float tiling) {
		MK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subtexture, tintColor, tiling, -1);

	}

	void Renderer2D::DrawTriangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation) {

		DrawTriangle({ position.x, position.y, 0.0f }, size, color, rotation);

	}

	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation) {
		MK_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f),
				{ size.x, size.y, 1.0f });
		s_Data.textureShader->setMat4("u_Transform", transform);

		s_Data.textureShader->setVec4("u_Color", color);
		s_Data.whiteTexture->bind();

		s_Data.triangleVertexArray->bind();
		RenderCommand::DrawIndexed(s_Data.triangleVertexArray);


	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityId) {

		// TODO: when SpriteRendererComponent supports textures, handle that branching here

		DrawQuad(transform, src.color, entityId);

	}

	void Renderer2D::ResetStats() {
		memset(&s_Data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return s_Data.stats;
	}



}
