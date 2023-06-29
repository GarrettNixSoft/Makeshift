#pragma once

#include "Entity.hpp"

namespace Makeshift {

	class ScriptableEntity {
	public:
		template<typename T>
		T& getComponent() {
			return m_Entity.getComponent<T>();
		}

	private:
		Entity m_Entity;

		friend class Scene;
	};

}