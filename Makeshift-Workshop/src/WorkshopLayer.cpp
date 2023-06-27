#include "mkpch.hpp"
#include "WorkshopLayer.hpp"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

namespace Makeshift {

	static const uint32_t s_MapWidth = 24;

	static const char* s_MapTiles =
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWDDDDDDDWWWWWWWWWW"
		"WWWWWDDDDDDDDDDDDWWWWWWW"
		"WWWWDDDDDDDDDDDDDDDDWWWW"
		"WWWDDDDWWWDDDDDDDCDDDWWW"
		"WWDDDDDWWWDDDDDDDDDDDDWW"
		"WDDDDDDDDDDDDDDDDDDDDDWW"
		"WWDDDDDDDDDDDDDDDDDDDDWW"
		"WWWDDDDDDDDDDDDDDDDDDDWW"
		"WWWWDDDDDDDDDDDDDDDDDDWW"
		"WWWWWDDDDDDDDDDDDDDDDWWW"
		"WWWWWWWDDDDDDDDDDDDDWWWW"
		"WWWWWWWWWDDDDDDDDDWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		;



	WorkshopLayer::WorkshopLayer() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f) {

	}

	void WorkshopLayer::onAttach() {
		MK_PROFILE_FUNCTION();

		FramebufferSpecification fbSpec;
		fbSpec.width = 1920;
		fbSpec.height = 1080;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_Texture = Texture2D::Create("assets/textures/checkerboard.png");
		m_SpriteSheet = Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

		m_StairsTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128.0f, 128.0f });
		m_BarrelTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128.0f, 128.0f });
		m_TreeTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128.0f, 128.0f }, { 1, 2 });

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

		s_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128.0f, 128.0f });
		s_TextureMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128.0f, 128.0f });

		m_CameraController.setZoomLevel(5.0f);
	}

	void WorkshopLayer::onDetach() {
		MK_PROFILE_FUNCTION();

		// Nothing
	}

	void WorkshopLayer::onUpdate(Timestep ts) {

		MK_PROFILE_FUNCTION();

		// Update
		m_CameraController.onUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		{
			MK_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->bind();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
		}

		#if 0
		{
			static float rotation = 0.0f;
			rotation += ts * 90.0f;

			MK_PROFILE_SCOPE("Render Scene");
			Renderer2D::BeginScene(cameraController.getCamera());

			Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.9f, 0.1f, 0.1f, 1.0f });
			Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.9f, 0.2f, 0.2f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, squareColor);
			Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, texture, { 1.0f, 0.8f, 0.8f, 1.0f }, 10.0f);
			Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), texture, { 1.0f, 0.8f, 0.8f, 1.0f }, 20.0f);

			//Renderer2D::DrawTriangle({ 1.0f, 1.0f }, { 0.6f, 0.6f }, { 0.1f, 0.3f, 0.9f, 1.0f });

			Renderer2D::EndScene();

			Renderer2D::BeginScene(cameraController.getCamera());

			for (float y = -5.0f; y < 5.0f; y += 0.5f) {
				for (float x = -5.0f; x < 5.0f; x += 0.5f) {
					glm::vec4 color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.2f, 0.7f };
					Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}

			Renderer2D::EndScene();
		}
		#endif

		Renderer2D::BeginScene(m_CameraController.getCamera());

		for (uint32_t y = 0; y < m_MapHeight; y++) {
			for (uint32_t x = 0; x < m_MapWidth; x++) {

				char tileType = s_MapTiles[y * m_MapWidth + x];
				Ref<SubTexture2D> texture;

				if (s_TextureMap.find(tileType) != s_TextureMap.end())
					texture = s_TextureMap[tileType];
				else
					texture = m_BarrelTexture;

				Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);

			}
		}

		//Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f}, stairsTexture);
		//Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.0f }, {1.0f, 1.0f}, barrelTexture);
		//Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, {1.0f, 2.0f}, treeTexture);

		Renderer2D::EndScene();
		m_Framebuffer->unbind();

	}

	void WorkshopLayer::onImGuiRender(Timestep ts) {
		MK_PROFILE_FUNCTION();

		static bool dockingEnabled = true;

		if (dockingEnabled) {
			static bool dockspaceOpen = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen) {
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			else {
				dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			if (!opt_padding)
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.

					if (ImGui::MenuItem("Exit")) Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
		}



		ImGui::Begin("Settings");

		ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_SquareColor));

		ImGui::End();

		ImGui::Begin("Viewport");

		uint32_t textureId = m_Framebuffer->getColorAttachmentRendererId();
		ImGui::Image((void*)textureId, ImVec2{ 1280.0f, 720.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

		ImGui::Begin("Stats");

		auto stats = Renderer2D::GetStats();
		int fps = (int)(1000.0f / ts.getMilliseconds());

		ImGui::Text("-- Renderer2D --");
		ImGui::Text("Frame Time: %.3fms (%d fps)", ts.getMilliseconds(), fps);
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		m_ProfileResults.clear();

		ImGui::End();



		ImGui::End();
	}

	void WorkshopLayer::onEvent(Event& e) {
		m_CameraController.onEvent(e);
	}

}
