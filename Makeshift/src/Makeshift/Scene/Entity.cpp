#include "mkpch.hpp"
#include "Entity.hpp"

namespace Makeshift {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {}

}
