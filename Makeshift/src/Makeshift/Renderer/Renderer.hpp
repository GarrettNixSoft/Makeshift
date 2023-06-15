#pragma once

#include "RenderCommand.hpp"
#include "OrthographicCamera.hpp"
#include "Shader.hpp"

namespace Makeshift {

	class Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera); // TODO
		static void EndScene(); // TODO

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* sceneData;
	};


}