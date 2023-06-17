#include "mkpch.hpp"
#include "Renderer2D.hpp"

#include "VertexArray.hpp"
#include "Shader.hpp"

#include "RenderCommand.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Makeshift {

	struct Renderer2DStorage {

		Ref<VertexArray> quadVertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() {

		s_Data = new Renderer2DStorage();

		s_Data->quadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->setLayout({
			{ ShaderDataType::Vec3, "position" },
			{ ShaderDataType::Vec2, "texCoord" }
		});
		s_Data->quadVertexArray->addVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->quadVertexArray->setIndexBuffer(squareIB);

		s_Data->whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		s_Data->textureShader = Shader::Create("assets/shaders/texture.glsl");
		s_Data->textureShader->bind();
		s_Data->textureShader->setInt("u_Texture", 0);

	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {

		s_Data->textureShader->bind();
		s_Data->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

	}

	void Renderer2D::EndScene() {

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation) {

		DrawQuad({ position.x, position.y, 0.0f }, size, color, rotation);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation) {

		//s_Data->textureShader->bind();

		s_Data->textureShader->setVec4("u_Color", color);
		s_Data->whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
		s_Data->textureShader->setMat4("u_Transform", transform);

		s_Data->quadVertexArray->bind();
		RenderCommand::DrawIndexed(s_Data->quadVertexArray);


	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& color, float rotation, float tiling) {

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, {1.0f,1.0f,1.0f,1.0f}, rotation, tiling);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4 & color, float rotation, float tiling) {

		//s_Data->textureShader->bind();

		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
		s_Data->textureShader->setMat4("u_Transform", transform);

		s_Data->textureShader->setFloat("u_TilingFactor", tiling);
		s_Data->textureShader->setVec4("u_Color", color);

		s_Data->quadVertexArray->bind();
		RenderCommand::DrawIndexed(s_Data->quadVertexArray);

	}



}