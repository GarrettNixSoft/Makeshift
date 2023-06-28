#include "mkpch.hpp"
#include "Scene.hpp"

#include "Components.hpp"
#include "Makeshift/Renderer/Renderer2D.hpp"

#include <glm/glm.hpp>

#include "Entity.hpp"

namespace Makeshift {

	Scene::Scene() {

	}

	Scene::~Scene() {

	}

	Entity Scene::createEntity(const std::string& name) {
		Entity entity = { m_Registry.create(), this };
		entity.addComponent<TransformComponent>();
		auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::onUpdate(Timestep ts) {
		MK_PROFILE_FUNCTION();

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group) {

				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.primary) {
					mainCamera = &camera.camera;
					cameraTransform = &transform.transform;
					break;
				}

			}
		}

		if (mainCamera) {
			Renderer2D::BeginScene(mainCamera->getProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {

				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.color);

			}

			Renderer2D::EndScene();
		}

		

	}

}
