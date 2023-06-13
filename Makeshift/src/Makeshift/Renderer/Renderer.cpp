#include "mkpch.hpp"
#include "Renderer.hpp"

#include "RenderCommand.hpp"
#include "OrthographicCamera.hpp"
#include "Shader.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Makeshift {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera) {
		sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::EndScene() {
		
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("viewProjection", sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("transform", transform);

		vertexArray->bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}