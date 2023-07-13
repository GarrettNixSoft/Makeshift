#include "SceneHeirarchyPanel.hpp"

#include "Makeshift/Scene/Components.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

namespace Makeshift {

	SceneHeirarchyPanel::SceneHeirarchyPanel(const Ref<Scene>& scene, const Ref<EditorContext>& context) {
		setContext(scene, context);
	}

	void SceneHeirarchyPanel::setContext(const Ref<Scene>& context, const Ref<EditorContext>& editorContext) {
		m_Context = context;
		m_SelectionContext = {};
		m_EditorContext = editorContext;
	}

	void SceneHeirarchyPanel::onImGuiRender() {

		ImGui::Begin("Scene Heirarchy");

		m_Context->m_Registry.each([&](auto entityId) {
			Entity entity{ entityId, m_Context.get() };
			drawEntityNode(entity);
		});

		// Clear selection if a click has not been consumed by any previous UI elements
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectionContext = {};
		}

		// Right clicking on blank space in the panel
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)) {

			if (ImGui::MenuItem("Create Empty Entity")) {
				m_Context->createEntity("Empty Entity");

				m_EditorContext->flagEdit();
			}

			ImGui::EndPopup();

		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext) {

			drawComponents(m_SelectionContext);

		}

		ImGui::End();

	}

	void SceneHeirarchyPanel::setSelectedEntity(Entity entity) {

		m_SelectionContext = entity;

	}

	void SceneHeirarchyPanel::drawEntityNode(Entity entity) {
		auto& tag = entity.getComponent<TagComponent>().tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
			// TODO: selection change callback
		}

		// use a boolean to defer deletion to the end of this function call
		bool deleteEntity = false;
		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::MenuItem("Delete Entity")) {
				deleteEntity = true;
			}

			ImGui::EndPopup();

		}

		if (opened) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)8675309, flags, tag.c_str());
			if (opened) {
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		// perform possible deferred deletion now that it's safe to do so
		if (deleteEntity) {{
				m_Context->destroyEntity(entity);
				m_EditorContext->flagEdit();
			}
			// clear selection if we just deleted the selected entity
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}

	}

	static bool drawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {

		bool edit = false;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0]; // fonts stored in the order they are loaded in ImGuiLayer (TODO: write a system for this)

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
			edit = true;
		}
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f")) edit = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
			edit = true;
		}
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f")) edit = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
			edit = true;
		}
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f")) edit = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return edit;

	}

	template<typename T, typename UIFunction>
	static void drawComponent(const std::string& label, Entity entity, const Ref<EditorContext>& editorContext, UIFunction uiFunction) {

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.hasComponent<T>()) {
			auto& component = entity.getComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());

			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) {
				ImGui::OpenPopup("ComponentSettings");
			}

			// defer removal if requested to the end of this function call
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove Component")) {
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open) {
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent) {
				entity.removeComponent<T>();
				editorContext->flagEdit();
			}

		}

	}

	void SceneHeirarchyPanel::drawComponents(Entity entity) {

		// Realistically, all entities will always have a tag, but better to check than crash.
		if (entity.hasComponent<TagComponent>()) {
			auto& tag = entity.getComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
				m_EditorContext->flagEdit();
			}
		}

		// Give a button to add components
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent")) {
			
			if (ImGui::MenuItem("Camera")) {
				m_SelectionContext.addComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
				m_EditorContext->flagEdit();
			}

			if (ImGui::MenuItem("Sprite Renderer")) {
				m_SelectionContext.addComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
				m_EditorContext->flagEdit();
			}

			ImGui::EndPopup();

		}

		ImGui::PopItemWidth();

		// Draw component UI!

		drawComponent<TransformComponent>("Transform", entity, m_EditorContext, [&](auto& component) {

			if (drawVec3Control("Translation", component.translation)) m_EditorContext->flagEdit();
			glm::vec3 rotation = glm::degrees(component.rotation);
			if (drawVec3Control("Rotation", rotation)) m_EditorContext->flagEdit();
			component.rotation = glm::radians(rotation);
			if (drawVec3Control("Scale", component.scale, 1.0f)) m_EditorContext->flagEdit();

		});

		drawComponent<CameraComponent>("Camera", entity, m_EditorContext, [&](auto& component) {

			auto& camera = component.camera;

			ImGui::Checkbox("Primary", &component.primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {

					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.setProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (camera.getProjectionType() == SceneCamera::ProjectionType::Perspective) {

				float perspectiveFOV = glm::degrees(camera.getPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &perspectiveFOV)) {
					camera.setPerspectiveVerticalFOV(glm::radians(perspectiveFOV));
					m_EditorContext->flagEdit();
				}

				float perspectiveNear = camera.getPerspectiveNearClip();
				if (ImGui::DragFloat("Near Clip", &perspectiveNear)) {
					camera.setPerspectiveNearClip(perspectiveNear);
					m_EditorContext->flagEdit();
				}

				float perspectiveFar = camera.getPerspectiveFarClip();
				if (ImGui::DragFloat("Far Clip", &perspectiveFar)) {
					camera.setPerspectiveFarClip(perspectiveFar);
					m_EditorContext->flagEdit();
				}

			}

			else if (camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic) {

				float orthoSize = camera.getOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize)) {
					camera.setOrthographicSize(orthoSize);
					m_EditorContext->flagEdit();
				}

				float orthoNear = camera.getOrthographicNearClip();
				if (ImGui::DragFloat("Near Clip", &orthoNear)) {
					camera.setOrthographicNearClip(orthoNear);
					m_EditorContext->flagEdit();
				}

				float orthoFar = camera.getOrthographicFarClip();
				if (ImGui::DragFloat("Far Clip", &orthoFar)) {
					camera.setOrthographicFarClip(orthoFar);
					m_EditorContext->flagEdit();
				}

				ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
			}

		});

		drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, m_EditorContext, [&](auto& component) {

			if (ImGui::ColorEdit4("Color", glm::value_ptr(component.color))) m_EditorContext->flagEdit();

		});

	}

}