#pragma once

#include "Makeshift/Core/Timestep.hpp"

#include <entt.hpp>

namespace Makeshift {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = std::string());

		void onUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
		
		friend class Entity;
	};

}
