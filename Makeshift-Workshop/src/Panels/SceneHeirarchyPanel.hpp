#pragma once

#include "Makeshift/Core/Core.hpp"
#include "Makeshift/Core/Logger.hpp"
#include "Makeshift/Scene/Scene.hpp"
#include "Makeshift/Scene/Entity.hpp"

#include "../EditorContext.hpp"

namespace Makeshift {

	class SceneHeirarchyPanel {
	public:
		SceneHeirarchyPanel() = default;
		SceneHeirarchyPanel(const Ref<Scene>& scene, const Ref<EditorContext>& context);

		void setContext(const Ref<Scene>& context, const Ref<EditorContext>& editorContext);

		void onImGuiRender();

		Entity getSelectedEntity() const { return m_SelectionContext; }
		void setSelectedEntity(Entity entity);
	private:
		void drawEntityNode(Entity entity);
		void drawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

		Ref<EditorContext> m_EditorContext;

	};

}
