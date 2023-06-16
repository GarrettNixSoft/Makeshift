#include "mkpch.hpp"
#include "Renderer2D.hpp"

#include "VertexArray.hpp"
#include "Shader.hpp"

#include "RenderCommand.hpp"

namespace Makeshift {

	struct Renderer2DStorage {

		Ref<VertexArray> quadVertexArray;
		Ref<Shader> flatColorShader;

	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() {

		s_Data = new Renderer2DStorage();

		s_Data->quadVertexArray = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->setLayout({
			{ ShaderDataType::Vec3, "position" }
		});
		s_Data->quadVertexArray->addVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		s_Data->quadVertexArray->setIndexBuffer(squareIB);

		s_Data->flatColorShader = Shader::Create("assets/shaders/flatColor.glsl");

	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {

		s_Data->flatColorShader->bind();
		s_Data->flatColorShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
		s_Data->flatColorShader->setMat4("u_Transform", glm::mat4(1.0f));

	}

	void Renderer2D::EndScene() {

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {

		DrawQuad({ position.x, position.y, 0.0f }, size, color);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {

		s_Data->flatColorShader->bind();
		s_Data->flatColorShader->setVec4("u_Color", color);
		s_Data->quadVertexArray->bind();
		RenderCommand::DrawIndexed(s_Data->quadVertexArray);


	}


}