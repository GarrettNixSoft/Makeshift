#include "mkpch.hpp"
#include "OrthographicCameraController.hpp"

#include "Makeshift/Core/Input.hpp"
#include "Makeshift/Core/Keycodes.hpp"

namespace Makeshift {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationAllowed)
		: m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.left, m_Bounds.right, m_Bounds.bottom, m_Bounds.top), m_RotationAllowed(rotationAllowed) {

	}

	void OrthographicCameraController::onUpdate(Timestep ts) {
		MK_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(MK_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(MK_KEY_D)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(MK_KEY_W)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(MK_KEY_S)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}
		if (m_RotationAllowed) {
			if (Input::IsKeyPressed(MK_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			if (Input::IsKeyPressed(MK_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}
			m_Camera.setRotation(m_CameraRotation);
		}

		m_Camera.setPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel * 3.0f;

	}

	void OrthographicCameraController::onEvent(Event& e) {
		MK_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(MK_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(MK_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));

	}

	void OrthographicCameraController::onResize(float width, float height) {
		m_AspectRatio = width / height;
		calculateView();
	}

	void OrthographicCameraController::calculateView() {
		MK_PROFILE_FUNCTION();

		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.setProjection(m_Bounds.left, m_Bounds.right, m_Bounds.bottom, m_Bounds.top);
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) {
		MK_PROFILE_FUNCTION();

		m_ZoomLevel -= e.getYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		calculateView();
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e) {
		MK_PROFILE_FUNCTION();

		onResize((float) e.getWidth(), (float) e.getHeight());
		return false;
	}

}