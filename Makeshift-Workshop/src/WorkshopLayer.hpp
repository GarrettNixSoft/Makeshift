#pragma once

#include <Makeshift.hpp>

namespace Makeshift {

	class WorkshopLayer : public Layer {
	public:
		WorkshopLayer();
		virtual ~WorkshopLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;

		void onUpdate(Timestep ts) override;
		virtual void onImGuiRender(Timestep ts) override;
		virtual void onEvent(Event& e) override;

	private:
		OrthographicCameraController m_CameraController;

		// TEMPORARY
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_StairsTexture;
		Ref<SubTexture2D> m_BarrelTexture;
		Ref<SubTexture2D> m_TreeTexture;

		struct ProfileResult {
			const char* name;
			float time;
		};

		std::vector<ProfileResult> m_ProfileResults;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

	};

}