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

	void Scene::destroyEntity(Entity entity) {
		m_Registry.destroy(entity);
	}

	void Scene::onUpdate(Timestep ts) {
		MK_PROFILE_FUNCTION();

		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {

				// TODO: move to Scen::onScenePlay() or something
				if (!nsc.instance) {
					nsc.instance = nsc.instantiateScript();
					nsc.instance->m_Entity = Entity{ entity, this };

					nsc.instance->onCreate();
				}
				nsc.instance->onUpdate(ts);
			});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {

				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.primary) {
					mainCamera = &camera.camera;
					cameraTransform = transform.getTransform();
					break;
				}

			}
		}

		// Render things if there's a primary camera present
		if (mainCamera) {
			Renderer2D::BeginScene(mainCamera->getProjection(), cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {

				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.getTransform(), sprite.color);

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

	template<typename T>
	void Scene::onComponentAdded(Entity entity, T& component) {
		static_assert(false);
	}

	template<>
	void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {
		// nothing
	}

	template<>
	void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		component.camera.setViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {
		// nothing
	}

	template<>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component) {
		// nothing
	}

	template<>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {
		// nothing
	}

}
