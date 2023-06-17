#include "mkpch.hpp"
#include "Renderer2D.hpp"

#include "VertexArray.hpp"
#include "Shader.hpp"

#include "RenderCommand.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Makeshift {

	struct Renderer2DStorage {

		Ref<VertexArray> quadVertexArray;
		Ref<Shader> flatColorShader;
		Ref<Shader> textureShader;

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

		s_Data->flatColorShader = Shader::Create("assets/shaders/flatColor.glsl");
		s_Data->textureShader = Shader::Create("assets/shaders/texture.glsl");
		
		s_Data->textureShader->bind();
		s_Data->textureShader->setInt("u_Texture", 0);

	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {

		s_Data->flatColorShader->bind();
		s_Data->flatColorShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

		s_Data->textureShader->bind();
		s_Data->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

	}

	void Renderer2D::EndScene() {

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation) {

		DrawQuad({ position.x, position.y, 0.0f }, size, color, rotation);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation) {

		s_Data->flatColorShader->bind();
		s_Data->flatColorShader->setVec4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
		s_Data->flatColorShader->setMat4("u_Transform", transform);

		s_Data->quadVertexArray->bind();
		RenderCommand::DrawIndexed(s_Data->quadVertexArray);


	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture, float rotation, float tiling, const glm::vec4& tintColor) {

		DrawQuad({ position.x, position.y, 0.0f }, size, texture, rotation, tiling);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, float rotation, float tiling, const glm::vec4& tintColor) {

		s_Data->textureShader->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
		s_Data->textureShader->setMat4("u_Transform", transform);

		s_Data->textureShader->setFloat("u_TilingFactor", tiling);
		s_Data->textureShader->setVec4("u_TintColor", tintColor);

		texture->bind();

		s_Data->quadVertexArray->bind();
		RenderCommand::DrawIndexed(s_Data->quadVertexArray);

	}



}