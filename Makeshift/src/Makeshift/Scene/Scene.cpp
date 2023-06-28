#include "mkpch.hpp"
#include "Scene.hpp"

#include "Components.hpp"
#include "Makeshift/Renderer/Renderer2D.hpp"

#include <glm/glm.hpp>

namespace Makeshift {

	Scene::Scene() {

	}

	Scene::~Scene() {

	}

	entt::entity Scene::createEntity() {
		return m_Registry.create();
	}

	void Scene::onUpdate(Timestep ts) {
		MK_PROFILE_FUNCTION();

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {

			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.color);

		}

	}

}
