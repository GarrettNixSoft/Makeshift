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

		const std::string& getName() const { return m_Name; }
		void setName(const std::string& name) { m_Name = name; }

		Entity getPrimaryCameraEntity();
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		std::string m_Name = "Untitled Scene";
		
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHeirarchyPanel;
	};

}
