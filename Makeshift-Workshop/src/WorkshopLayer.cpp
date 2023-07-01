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

		m_ActiveScene = CreateRef<Scene>();

		// Entities
		m_GreenSquare = m_ActiveScene->createEntity("Green Square");
		m_GreenSquare.addComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		m_RedSquare = m_ActiveScene->createEntity("Red Square");
		m_RedSquare.addComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_CameraEntity = m_ActiveScene->createEntity("Camera Entity");
		m_CameraEntity.addComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->createEntity("Other Camera");
		auto& cc = m_SecondCamera.addComponent<CameraComponent>();
		cc.primary = false;

		class CameraController : public ScriptableEntity {
		public:
			virtual void onCreate() {
				auto& translation = getComponent<TransformComponent>().translation;
				translation.x = rand() % 10 - 5.0f;
			}

			virtual void onDestroy() {

			}

			virtual void onUpdate(Timestep ts) {
				auto& translation = getComponent<TransformComponent>().translation;

				float speed = 5.0f;

				if (Input::isKeyPressed(KeyCode::A))
					translation.x -= speed * ts;
				if (Input::isKeyPressed(KeyCode::D))
					translation.x += speed * ts;
				if (Input::isKeyPressed(KeyCode::W))
					translation.y += speed * ts;
				if (Input::isKeyPressed(KeyCode::S))
					translation.y -= speed * ts;
			}
		};

		m_CameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
		m_SecondCamera.addComponent<NativeScriptComponent>().bind<CameraController>();

		m_SceneHeirarchyPanel.setContext(m_ActiveScene);
		
	}

	void WorkshopLayer::onDetach() {
		MK_PROFILE_FUNCTION();

		// Nothing
	}

	void WorkshopLayer::onUpdate(Timestep ts) {

		MK_PROFILE_FUNCTION();

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->getSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y)) {
			m_Framebuffer->resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.onResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update
		if (m_ViewportFocused)
			m_CameraController.onUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		{
			MK_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->bind();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
		}

		{
			MK_PROFILE_SCOPE("Render Scene (ECS)");

			// Update scene
			m_ActiveScene->onUpdate(ts);

			m_Framebuffer->unbind();
		}

	}

	void WorkshopLayer::onImGuiRender(Timestep ts) {
		MK_PROFILE_FUNCTION();

		// ================================ DOCK SPACE ================================
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
		// ================================ DOCK SPACE ================================

		// ================================ SCENE HEIRARCHY PANEL ================================
		m_SceneHeirarchyPanel.OnImGuiRender();
		// ================================ SCENE HEIRARCHY PANEL ================================

		// ================================ SETTINGS ================================
		ImGui::Begin("Settings");

		ImGui::Text("TODO");

		ImGui::End();
		// ================================ SETTINGS ================================

		// ================================ RENDER STATS ================================
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
		// ================================ RENDER STATS ================================

		// ================================ VIEWPORT ================================
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->blockEvents(!(m_ViewportFocused && m_ViewportHovered));

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize)) {
			//m_Framebuffer->resize((uint32_t) viewportPanelSize.x, (uint32_t) viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			//m_CameraController.onResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		uint32_t textureId = m_Framebuffer->getColorAttachmentRendererId();
		ImGui::Image((void*)textureId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
		// ================================ VIEWPORT ================================

		ImGui::End();
	}

	void WorkshopLayer::onEvent(Event& e) {
		m_CameraController.onEvent(e);
	}

}
