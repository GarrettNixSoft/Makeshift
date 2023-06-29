#include "SceneHeirarchyPanel.hpp"

#include <imgui/imgui.h>

#include "Makeshift/Scene/Components.hpp"

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

}