#pragma once

#include "Makeshift/Layer.hpp"
#include "Makeshift/Events/KeyEvent.hpp"
#include "Makeshift/Events/MouseEvent.hpp"

namespace Makeshift {

	class MK_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& event);

		bool onKeyPressed(KeyPressedEvent& event);
		bool onKeyReleased(KeyReleasedEvent& event);
		bool onMouseButtonPressed(MouseButtonPressedEvent& event);
		bool onMouseButtonReleased(MouseButtonReleasedEvent& event);
		bool onMouseMoved(MouseMovedEvent& event);
		bool onMouseScrolled(MouseScrolledEvent& event);
	private:
		float lastTime = 0.0f;
	};

}
