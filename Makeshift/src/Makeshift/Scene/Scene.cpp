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

		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				if (!nsc.instance) {
					nsc.instantiateFunction();
					nsc.instance->m_Entity = Entity{ entity, this };

					if (nsc.onCreateFunction)
						nsc.onCreateFunction(nsc.instance);
				}
				if (nsc.onUpdateFunction)
					nsc.onUpdateFunction(nsc.instance, ts);
			});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {

				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

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

	void Scene::onViewportResize(uint32_t width, uint32_t height) {
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize non-fixed aspect ratio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {

			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio) {
				
				cameraComponent.camera.setViewportSize(width, height);

			}

		}
	}

}
