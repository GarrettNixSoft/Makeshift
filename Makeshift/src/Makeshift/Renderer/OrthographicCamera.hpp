#pragma once

#include <glm/glm.hpp>

namespace Makeshift {

	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void setProjection(float left, float right, float bottom, float top);

		const glm::vec3& getPosition() const { return position; }
		void setPosition(const glm::vec3& pos) { position = pos; recalculateViewMatrix(); }

		float getRotation() const { return rotation; }
		void setRotation(float rot) { rotation = rot; recalculateViewMatrix(); }

		const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		const glm::mat4& getViewMatrix() const { return viewMatrix; }
		const glm::mat4& getViewProjectionMatrix() const { return viewProjectionMatrix; }

	private:
		void recalculateViewMatrix();
	private:
		glm::mat4 projectionMatrix{ 1.0f };
		glm::mat4 viewMatrix{ 1.0f };
		glm::mat4 viewProjectionMatrix{ 1.0f };

		glm::vec3 position{};
		float rotation = 0.0f;
	};

}