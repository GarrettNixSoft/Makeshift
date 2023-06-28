#pragma once

#include "Makeshift/Renderer/Camera.hpp"

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

		Makeshift::Camera camera;
		bool primary = true; // TODO move to Scene

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection) : camera( projection ) {}

	};

}