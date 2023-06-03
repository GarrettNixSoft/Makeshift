#pragma once

#include "Makeshift/Core.hpp"

namespace Makeshift {

	class MK_API Input {
	public:
		inline static bool isKeyPressed(int keycode) { return instance->isKeyPressedImpl(keycode); }

		inline static bool isMouseButtonPressed(int button) { return instance->isMouseButtonPressedImpl(button); }
		inline static float getMouseX() { return instance->getMouseXImpl(); }
		inline static float getMouseY() { return instance->getMouseYImpl(); }
		inline static std::pair<float, float> getMousePosition() { return instance->getMousePositionImpl(); }
		inline static float getMouseScrollX() { return instance->getMouseScrollXImpl(); }
		inline static float getMouseScrollY() { return instance->getMouseScrollYImpl(); }
	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;

		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;
		virtual float getMouseScrollXImpl() = 0;
		virtual float getMouseScrollYImpl() = 0;
	private:
		static Input* instance;
	};

}