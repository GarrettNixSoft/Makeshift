#pragma once

#include "Makeshift/Core/Core.hpp"
#include "Makeshift/Core/Logger.hpp"
#include "Makeshift/Scene/Scene.hpp"
#include "Makeshift/Scene/Entity.hpp"

namespace Makeshift {

	class SceneHeirarchyPanel {
	public:
		SceneHeirarchyPanel() = default;
		SceneHeirarchyPanel(const Ref<Scene>& scene);

		void setContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void drawEntityNode(Entity entity);
		void drawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

	};

}
