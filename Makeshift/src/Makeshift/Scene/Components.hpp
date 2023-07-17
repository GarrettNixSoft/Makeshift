#pragma once

#include "Makeshift/Scene/SceneCamera.hpp"

#include "Makeshift/Renderer/Texture.hpp"
#include "Makeshift/Renderer/SubTexture2D.hpp"

#include "ScriptableEntity.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Makeshift {

	struct TagComponent {
		
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& t) : tag(t) {}

	};

	struct TransformComponent {

		glm::vec3 translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& tl) : translation(tl) {}

		glm::mat4 getTransform() const {
			glm::mat4 rotate = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)
				* rotate
				* glm::scale(glm::mat4(1.0f), scale);
		}

	};

	struct SpriteRendererComponent {

		glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
		Ref<Texture2D> texture;
		float tilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& clr) : color(clr) {}

	};

	struct SpriteSubtextureRendererComponent {

		glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
		Ref<SubTexture2D> texture;
		float tilingFactor = 1.0f;

		SpriteSubtextureRendererComponent() = default;
		SpriteSubtextureRendererComponent(const SpriteSubtextureRendererComponent&) = default;
		SpriteSubtextureRendererComponent(const glm::vec4& clr) : color(clr) {}

	};

	struct CameraComponent {

		Makeshift::SceneCamera camera;
		bool primary = true; // TODO move to Scene
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

	};

	struct NativeScriptComponent {

		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*instantiateScript)();
		void (*destroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind() {
			instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}

	};

}
