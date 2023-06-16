#pragma once

#include "Makeshift/Renderer/OrthographicCamera.hpp"
#include "Makeshift/Core/Timestep.hpp"

#include "Makeshift/Events/ApplicationEvent.hpp"
#include "Makeshift/Events/MouseEvent.hpp"

#include <glm/glm.hpp>

namespace Makeshift {

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotationAllowed = false);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		OrthographicCamera& getCamera() { return m_Camera; }
		const OrthographicCamera& getCamera() const { return m_Camera; }

		float getZoomLevel() const { return m_ZoomLevel; }
		void setZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_RotationAllowed;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 90.0f;
	};

}