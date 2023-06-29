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

		std::function<void()> instantiateFunction;
		std::function<void()> destroyInstanceFunction;

		std::function<void(ScriptableEntity*)> onCreateFunction;
		std::function<void(ScriptableEntity*)> onDestroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> onUpdateFunction;

		template<typename T>
		void bind() {
			instantiateFunction = [&]() { instance = new T(); };
			destroyInstanceFunction = [&]() { delete (T*)instance; };

			onCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->onCreate(); };
			onDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->onDestroy(); };
			onUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->onUpdate(ts); };
		}

	};

}