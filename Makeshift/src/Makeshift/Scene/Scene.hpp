#pragma once

#include "Makeshift/Core/Timestep.hpp"

#include <entt.hpp>

namespace Makeshift {

	class Scene {
	public:
		Scene();
		~Scene();

		entt::entity createEntity();

		// TEMP
		entt::registry& reg() { return m_Registry; }

		void onUpdate(Timestep ts);
	private:
		entt::registry m_Registry;

	};

}
