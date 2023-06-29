#pragma once

#include "Makeshift/Scene/SceneCamera.hpp"

#include "ScriptableEntity.hpp"

#include <glm/glm.hpp>

namespace Makeshift {

	struct TagComponent {
		
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& t) : tag(t) {}

	};

	struct TransformComponent {

		glm::mat4 transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& tf) : transform(tf) {}

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }

	};

	struct SpriteRendererComponent {

		glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& clr) : color(clr) {}

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