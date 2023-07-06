#pragma once

#include "Makeshift/Core/Core.hpp"
#include "Makeshift/Core/Keycodes.hpp"
#include "Makeshift/Core/MouseCodes.hpp"

namespace Makeshift {

	class MK_API Input {
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool isMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> getMousePosition();

		static float getMouseScrollX();
		static float getMouseScrollY();
	};

}