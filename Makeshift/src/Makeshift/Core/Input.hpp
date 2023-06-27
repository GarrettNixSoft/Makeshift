#pragma once

#include "Makeshift/Core/Core.hpp"
#include "Makeshift/Core/Keycodes.hpp"

namespace Makeshift {

	class MK_API Input {
	public:
		static bool isKeyPressed(KeyCode keycode);

		static bool isMouseButtonPressed(int button);
		static float getMouseX();
		static float getMouseY();
		static std::pair<float, float> getMousePosition();

		static float getMouseScrollX();
		static float getMouseScrollY();
	};

}