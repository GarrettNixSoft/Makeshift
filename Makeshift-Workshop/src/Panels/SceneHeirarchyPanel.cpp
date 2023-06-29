#include "SceneHeirarchyPanel.hpp"

#include "Makeshift/Scene/Components.hpp"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Makeshift {

	SceneHeirarchyPanel::SceneHeirarchyPanel(const Ref<Scene>& scene) {
		setContext(scene);
	}

	void SceneHeirarchyPanel::setContext(const Ref<Scene>& context) {
		m_Context = context;
	}

	void SceneHeirarchyPanel::OnImGuiRender() {

		ImGui::Begin("Scene Heirarchy");

		m_Context->m_Registry.each([&](auto entityId) {
			Entity entity{ entityId, m_Context.get() };
			drawEntityNode(entity);
		});

		// Clear selection if a click has not been consumed by any previous UI elements
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext) {
			drawComponents(m_SelectionContext);
		}

		ImGui::End();

	}

	void SceneHeirarchyPanel::drawEntityNode(Entity entity) {
		auto& tag = entity.getComponent<TagComponent>().tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
			// TODO: selection change callback
		}
		if (opened) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)8675309, flags, tag.c_str());
			if (opened) {
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}

	void SceneHeirarchyPanel::drawComponents(Entity entity) {

		if (entity.hasComponent<TagComponent>()) {
			auto& tag = entity.getComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		if (entity.hasComponent<TransformComponent>()) {

			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = entity.getComponent<TransformComponent>().transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				ImGui::TreePop();
			}

			
		}

	}

}