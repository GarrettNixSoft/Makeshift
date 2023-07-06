#pragma once

#include <cstdint>

namespace Makeshift {

	using MouseCode = uint16_t;

	namespace Mouse {

		enum : MouseCode {
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,

			ButtonLast = Button7,
			ButtonLeft = Button0,
			ButtonRight = Button1,
			ButtonMiddle = Button2
		};

	}

}

// From glfw3.h
#define MK_MOUSE_BUTTON_1         0
#define MK_MOUSE_BUTTON_2         1
#define MK_MOUSE_BUTTON_3         2
#define MK_MOUSE_BUTTON_4         3
#define MK_MOUSE_BUTTON_5         4
#define MK_MOUSE_BUTTON_6         5
#define MK_MOUSE_BUTTON_7         6
#define MK_MOUSE_BUTTON_8         7
#define MK_MOUSE_BUTTON_LAST      MK_MOUSE_BUTTON_8
#define MK_MOUSE_BUTTON_LEFT      MK_MOUSE_BUTTON_1
#define MK_MOUSE_BUTTON_RIGHT     MK_MOUSE_BUTTON_2
#define MK_MOUSE_BUTTON_MIDDLE    MK_MOUSE_BUTTON_3