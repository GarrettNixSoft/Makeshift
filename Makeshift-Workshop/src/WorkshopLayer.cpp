#include "mkpch.hpp"
#include "WorkshopLayer.hpp"
#include "imgui/imgui.h"

#include "Makeshift/Scene/SceneSerializer.hpp"

#include "Makeshift/Utils/PlatformUtils.hpp"

#include "Makeshift/Math/Math.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo.h>

#include <sstream>
#include <filesystem>

namespace Makeshift {

	WorkshopLayer::WorkshopLayer() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f) {

	}

	void WorkshopLayer::onAttach() {
		MK_PROFILE_FUNCTION();

		FramebufferSpecification fbSpec;
		fbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		fbSpec.width = 1920;
		fbSpec.height = 1080;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1, 1000.0f);
		m_EditorContext = CreateRef<EditorContext>();

#if 0
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
#endif

		m_SceneHeirarchyPanel.setContext(m_ActiveScene, m_EditorContext);

		m_EditorContext->editCallback = [=]() { markTitleEditStatus(); };
		m_EditorContext->saveCallback = [=]() { markTitleEditStatus(); };

		markTitleEditStatus();
		
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
		if (m_ViewportFocused) {
			m_CameraController.onUpdate(ts);
			m_EditorCamera.onUpdate(ts);
		}

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
			m_ActiveScene->onUpdateEditor(ts, m_EditorCamera);

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
		ImGuiStyle& style = ImGui::GetStyle();

		float minWinSize = style.WindowMinSize.x; // grab and modify the minimium window size
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSize; // set it back to the default

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.

				if (ImGui::MenuItem("New...", "Ctrl+N")) {
					if (m_EditorContext->sceneEdited)
						m_ShowConfirmNewModal = true;
					else
						newScene();
				}
					

				ImGui::Separator();

				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					if (m_EditorContext->sceneEdited)
						m_ShowConfirmOpenModal = true;
					else
						openScene();
				}

				if (ImGui::MenuItem("Open Recent")) {
					// TODO
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					saveScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					saveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) {
					if (m_EditorContext->sceneEdited)
						m_ShowConfirmExitModal = true;
					else
						Application::Get().Close();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		
		// ================================ DOCK SPACE ================================

		// ================================ SCENE HEIRARCHY PANEL ================================
		m_SceneHeirarchyPanel.OnImGuiRender();
		// ================================ SCENE HEIRARCHY PANEL ================================

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
		Application::Get().GetImGuiLayer()->blockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize)) {
			//m_Framebuffer->resize((uint32_t) viewportPanelSize.x, (uint32_t) viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_EditorCamera.setViewportSize(viewportPanelSize.x, viewportPanelSize.y);
			//m_CameraController.onResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		uint32_t textureId = m_Framebuffer->getColorAttachmentRendererId();
		ImGui::Image((void*)textureId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Gizmo stuff
		Entity selectedEntity = m_SceneHeirarchyPanel.getSelectedEntity();
		if (selectedEntity && m_GizmoType != -1) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Use Editor Camera
			const glm::mat4& cameraProjection = m_EditorCamera.getProjection();
			glm::mat4 cameraView = m_EditorCamera.getViewMatrix();

			// Entity's transform
			auto& tc = selectedEntity.getComponent<TransformComponent>();
			glm::mat4 transform = tc.getTransform();

			// Gizmo Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for rotate/scale
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 15.0f; // Snap to 15deg when rotating

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION) m_GizmoType,
				ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {

				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				tc.translation = translation;
				tc.rotation = rotation; // using a delta supposedly prevents gimbal lock here, but does it really?
				tc.scale = scale;

				// flag the scene as having been edited
				m_EditorContext->flagEdit();

			}
		}


		ImGui::End();
		ImGui::PopStyleVar();
		// ================================ VIEWPORT ================================

		// End Dockspace
		ImGui::End();

		// ================================ SETTINGS ================================
		ImGui::Begin("Settings");

		ImGui::Text("Scene Name");

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), m_ActiveScene->getName().c_str());

		if (ImGui::InputText("##SceneName", buffer, sizeof(buffer))) {
			m_ActiveScene->setName(std::string(buffer));

			m_EditorContext->flagEdit();

			markTitleEditStatus();
		}

		{
			if (m_ShowConfirmNewModal)
				ImGui::OpenPopup("Confirm New Scene");

			// Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Confirm New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text("You have unsaved changes.");
				ImGui::Spacing();
				ImGui::Text("Discard them and open a new scene anyway?");
				ImGui::Separator();
				if (ImGui::Button("Yes")) {
					newScene();
					ImGui::CloseCurrentPopup();
					m_ShowConfirmNewModal = false;
				}
				ImGui::SameLine();
				ImGui::SetItemDefaultFocus();
				if (ImGui::Button("Cancel")) {
					ImGui::CloseCurrentPopup();
					m_ShowConfirmNewModal = false;
				}
				ImGui::EndPopup();
			}
		}

		{
			if (m_ShowConfirmOpenModal)
				ImGui::OpenPopup("Confirm Open Scene");

			// Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Confirm Open Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text("You have unsaved changes.");
				ImGui::Spacing();
				ImGui::Text("Discard them and open a new scene anyway?");
				ImGui::Separator();
				if (ImGui::Button("Yes")) {
					openScene();
					ImGui::CloseCurrentPopup();
					m_ShowConfirmOpenModal = false;
				}
				ImGui::SameLine();
				ImGui::SetItemDefaultFocus();
				if (ImGui::Button("Cancel")) {
					ImGui::CloseCurrentPopup();
					m_ShowConfirmOpenModal = false;
				}
				ImGui::EndPopup();
			}
		}

		{
			if (m_ShowConfirmExitModal)
				ImGui::OpenPopup("Confirm Exit");

			// Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Confirm Exit", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text("You have unsaved changes.");
				ImGui::Spacing();
				ImGui::Text("Discard them and exit anyway?");
				ImGui::Separator();
				if (ImGui::Button("Yes")) {
					Application::Get().Close();
					ImGui::CloseCurrentPopup();
					m_ShowConfirmExitModal = false;
				}
				ImGui::SameLine();
				ImGui::SetItemDefaultFocus();
				if (ImGui::Button("Cancel")) {
					ImGui::CloseCurrentPopup();
					m_ShowConfirmExitModal = false;
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
		// ================================ SETTINGS ================================

	}

	void WorkshopLayer::onEvent(Event& e) {
		m_CameraController.onEvent(e);
		m_EditorCamera.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(MK_BIND_EVENT_FN(WorkshopLayer::onKeyPressed));
		dispatcher.dispatch<WindowCloseEvent>(MK_BIND_EVENT_FN(WorkshopLayer::onWindowClosed));
	}

	bool WorkshopLayer::onKeyPressed(KeyPressedEvent& e) {

		// Repeatable key actions would go here, above the shortcut checks

		// Shortcuts
		if (e.getRepeatCount() > 0)
			return false;

		bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.getKeyCode()) {

			case Key::N: {
				if (ctrl)
					if (m_EditorContext->sceneEdited)
						m_ShowConfirmNewModal = true;
					else
						newScene();

				break;
			}

			case Key::O: {
				if (ctrl)
					if (m_EditorContext->sceneEdited)
						m_ShowConfirmOpenModal = true;
					else
						openScene();

				break;
			}

			case Key::S: {
				if (ctrl && shift)
					saveSceneAs();
				else if (ctrl)
					saveScene();

				break;
			}

			// Gizmos
			case Key::Q: {
				m_GizmoType = -1;
				break;
			}

			case Key::T: {
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}

			case Key::R: {
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}

			case Key::G: {
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
			
		}

	}

	bool WorkshopLayer::onWindowClosed(WindowCloseEvent& e) {
		if (m_EditorContext->sceneEdited)
			m_ShowConfirmExitModal = true;
		return true;
	}

	void WorkshopLayer::newScene() {

		// Create a new scene to load into (clear)
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHeirarchyPanel.setContext(m_ActiveScene, m_EditorContext);

		// new scene, remove edit flag
		m_EditorContext->clearEdit();

		// clear the context filepath
		m_EditorContext->activeSceneFilePath = std::string();
		
	}

	void WorkshopLayer::openScene() {
		std::string filepath = FileDialogs::OpenFile("Makeshift Scene (*.mkshft)\0*.mkshft\0");

		if (!filepath.empty()) {
			// Create a new scene to load into (clear)
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHeirarchyPanel.setContext(m_ActiveScene, m_EditorContext);

			SceneSerializer serializer(m_ActiveScene);
			serializer.deserialize(filepath);

			// hold onto the filepath we just loaded
			m_EditorContext->activeSceneFilePath = filepath;

			// newly opened scene, remove edit flag
			m_EditorContext->clearEdit();
		}
	}

	void WorkshopLayer::saveScene() {
		
		// If the context contains a filepath for the active scene, save to that scene
		if (!m_EditorContext->activeSceneFilePath.empty()) {

			// confirm the file exists
			std::filesystem::path f{ m_EditorContext->activeSceneFilePath };
			if (std::filesystem::exists(f)) {

				SceneSerializer serializer(m_ActiveScene);
				serializer.serialize(m_EditorContext->activeSceneFilePath);

				// changes saved, remove edit flag
				m_EditorContext->clearEdit();

			}

			else saveSceneAs();
			
		}
		else saveSceneAs();

	}

	void WorkshopLayer::saveSceneAs() {

		std::string filepath = FileDialogs::SaveFile("Makeshift Scene (*.mkshft)\0*.mkshft\0");

		if (!filepath.empty()) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.serialize(filepath);

			// hold onto the filepath we just saved to
			m_EditorContext->activeSceneFilePath = filepath;
			
			// changes saved, remove edit flag
			m_EditorContext->clearEdit();
		}

	}

	void WorkshopLayer::markTitleEditStatus() {

		std::stringstream strStream;
		strStream << "Makeshift Workshop -- " <<  m_ActiveScene->getName();
		if (m_EditorContext->sceneEdited) strStream << "*";

		Application::Get().getWindow().setTitle(strStream.str());

	}

}
