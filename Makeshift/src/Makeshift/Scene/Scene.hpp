#pragma once

#include "Makeshift/Core/Timestep.hpp"

#include "Makeshift/Renderer/EditorCamera.hpp"

#include <entt.hpp>

namespace Makeshift {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = std::string());
		void destroyEntity(Entity entity);

		void onUpdateRuntime(Timestep ts);
		void onUpdateEditor(Timestep ts, EditorCamera& camera);
		void onViewportResize(uint32_t width, uint32_t height);

		Entity getPrimaryCameraEntity();
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHeirarchyPanel;
	};

}
