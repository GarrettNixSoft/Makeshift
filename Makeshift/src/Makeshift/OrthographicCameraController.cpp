#include "mkpch.hpp"
#include "OrthographicCameraController.hpp"

#include "Makeshift/Input.hpp"
#include "Makeshift/Keycodes.hpp"

namespace Makeshift {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationAllowed)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_RotationAllowed(rotationAllowed) {

	}

	void OrthographicCameraController::onUpdate(Timestep ts) {

		if (Input::isKeyPressed(MK_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		if (Input::isKeyPressed(MK_KEY_D)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}
		if (Input::isKeyPressed(MK_KEY_W)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		if (Input::isKeyPressed(MK_KEY_S)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}
		if (m_RotationAllowed) {
			if (Input::isKeyPressed(MK_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			if (Input::isKeyPressed(MK_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}
			m_Camera.setRotation(m_CameraRotation);
		}

		m_Camera.setPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel * 3.0f;

	}

	void OrthographicCameraController::onEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(MK_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(MK_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));

	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.getYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e) {
		m_AspectRatio = (float) e.getWidth() / (float) e.getHeight();
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}