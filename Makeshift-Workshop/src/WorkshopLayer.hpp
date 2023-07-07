#pragma once

#include <Makeshift.hpp>

#include "Panels/SceneHeirarchyPanel.hpp"

#include "Makeshift/Renderer/EditorCamera.hpp"

#include "EditorContext.hpp"

#include <string>

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
		bool onKeyPressed(KeyPressedEvent& e);
		bool onWindowClosed(WindowCloseEvent& e);

		void newScene();
		void openScene();
		void saveScene();
		void saveSceneAs();

		void markTitleEditStatus();
	private:
		OrthographicCameraController m_CameraController;

		// TEMPORARY
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_GreenSquare;
		Entity m_RedSquare;
		Entity m_CameraEntity;
		Entity m_SecondCamera;
		
		bool m_ClipSpaceCamera = false;

		EditorCamera m_EditorCamera;

		Ref<EditorContext> m_EditorContext;

		struct ProfileResult {
			const char* name;
			float time;
		};

		std::vector<ProfileResult> m_ProfileResults;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		int m_GizmoType = -1;

		// Panels
		SceneHeirarchyPanel m_SceneHeirarchyPanel;

		// Modals
		bool m_ShowConfirmNewModal = false;
		bool m_ShowConfirmOpenModal = false;
		bool m_ShowConfirmExitModal = false;

	};

}