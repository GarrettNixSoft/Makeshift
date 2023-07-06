#pragma once

#include <cstdint>

namespace Makeshift {

	using KeyCode = uint16_t;

	namespace Key {

		enum : KeyCode {

			Space = 32,
			Apostrophe = 39,
			Comma = 44,
			Minus = 45,
			Period = 46,
			Slash = 47,

			D0 = 48,
			D1 = 49,
			D2 = 50,
			D3 = 51,
			D4 = 52,
			D5 = 53,
			D6 = 54,
			D7 = 55,
			D8 = 56,
			D9 = 57,

			Semicolon = 58,
			Equal = 59,

			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,

			LeftBracket = 91,
			Backslash = 92,
			RightBracket = 93,
			GraveAccent = 94,

			World1 = 161,
			World2 = 162,

			Escape = 256,
			Enter = 257,
			Tab = 258,
			Backspace = 259,
			Insert = 260,
			Delete = 261,
			Right = 262,
			Left = 263,
			Down = 264,
			Up = 265,
			PageUp = 266,
			PageDown = 267,
			Home = 268,
			End = 269,
			CapsLock = 280,
			ScrollLock = 281,
			NumLock = 282,
			PrintScreen = 283,
			Pause = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,

			KP0 = 320,
			KP1 = 321,
			KP2 = 322,
			KP3 = 323,
			KP4 = 324,
			KP5 = 325,
			KP6 = 326,
			KP7 = 327,
			KP8 = 328,
			KP9 = 329,
			KPDecimal = 330,
			KPDivide = 331,
			KPMultiply = 332,
			KPSubtract = 333,
			KPAdd = 334,
			KPEnter = 335,
			KPEqual = 336,

			LeftShift = 340,
			LeftControl = 341,
			LeftAlt = 342,
			LeftSuper = 343,
			RightShift = 344,
			RightControl = 345,
			RightAlt = 346,
			RightSuper = 347,
			Menu = 348

		};

	}

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode) {
		os << static_cast<int32_t>(keyCode);
		return os;
	}

}

// From glfw3.h
#define MK_KEY_SPACE              ::Makeshift::Key::Space
#define MK_KEY_APOSTROPHE         ::Makeshift::Key::Apostrophe		/* ' */
#define MK_KEY_COMMA              ::Makeshift::Key::Comma			/* , */
#define MK_KEY_MINUS              ::Makeshift::Key::Minus			/* - */
#define MK_KEY_PERIOD             ::Makeshift::Key::Period			/* . */
#define MK_KEY_SLASH              ::Makeshift::Key::Slash			/* / */
#define MK_KEY_0                  ::Makeshift::Key::D0
#define MK_KEY_1                  ::Makeshift::Key::D1
#define MK_KEY_2                  ::Makeshift::Key::D2
#define MK_KEY_3                  ::Makeshift::Key::D3
#define MK_KEY_4                  ::Makeshift::Key::D4
#define MK_KEY_5                  ::Makeshift::Key::D5
#define MK_KEY_6                  ::Makeshift::Key::D6
#define MK_KEY_7                  ::Makeshift::Key::D7
#define MK_KEY_8                  ::Makeshift::Key::D8
#define MK_KEY_9                  ::Makeshift::Key::D9
#define MK_KEY_SEMICOLON          ::Makeshift::Key::Semicolon		/* ; */
#define MK_KEY_EQUAL              ::Makeshift::Key::Equal			/* = */
#define MK_KEY_A                  ::Makeshift::Key::A
#define MK_KEY_B                  ::Makeshift::Key::B
#define MK_KEY_C                  ::Makeshift::Key::C
#define MK_KEY_D                  ::Makeshift::Key::D
#define MK_KEY_E                  ::Makeshift::Key::E
#define MK_KEY_F                  ::Makeshift::Key::F
#define MK_KEY_G                  ::Makeshift::Key::G
#define MK_KEY_H                  ::Makeshift::Key::H
#define MK_KEY_I                  ::Makeshift::Key::I
#define MK_KEY_J                  ::Makeshift::Key::J
#define MK_KEY_K                  ::Makeshift::Key::K
#define MK_KEY_L                  ::Makeshift::Key::L
#define MK_KEY_M                  ::Makeshift::Key::M
#define MK_KEY_N                  ::Makeshift::Key::N
#define MK_KEY_O                  ::Makeshift::Key::O
#define MK_KEY_P                  ::Makeshift::Key::P
#define MK_KEY_Q                  ::Makeshift::Key::Q
#define MK_KEY_R                  ::Makeshift::Key::R
#define MK_KEY_S                  ::Makeshift::Key::S
#define MK_KEY_T                  ::Makeshift::Key::T
#define MK_KEY_U                  ::Makeshift::Key::U
#define MK_KEY_V                  ::Makeshift::Key::V
#define MK_KEY_W                  ::Makeshift::Key::W
#define MK_KEY_X                  ::Makeshift::Key::X
#define MK_KEY_Y                  ::Makeshift::Key::Y
#define MK_KEY_Z                  ::Makeshift::Key::Z
#define MK_KEY_LEFT_BRACKET       ::Makeshift::Key::LeftBracket		/* [ */
#define MK_KEY_BACKSLASH          ::Makeshift::Key::Backslash		/* \ */
#define MK_KEY_RIGHT_BRACKET      ::Makeshift::Key::RightBracket	/* ] */
#define MK_KEY_GRAVE_ACCENT       ::Makeshift::Key::GraveAccent		/* ` */
#define MK_KEY_WORLD_1            ::Makeshift::Key::World1			/* non-US #1 */
#define MK_KEY_WORLD_2            ::Makeshift::Key::World2			/* non-US #2 */

/* Function keys */
#define MK_KEY_ESCAPE             ::Makeshift::Key::Escape
#define MK_KEY_ENTER              ::Makeshift::Key::Enter
#define MK_KEY_TAB                ::Makeshift::Key::Tab
#define MK_KEY_BACKSPACE          ::Makeshift::Key::Backspace
#define MK_KEY_INSERT             ::Makeshift::Key::Insert
#define MK_KEY_DELETE             ::Makeshift::Key::Delete
#define MK_KEY_RIGHT              ::Makeshift::Key::Right
#define MK_KEY_LEFT               ::Makeshift::Key::Left
#define MK_KEY_DOWN               ::Makeshift::Key::Down
#define MK_KEY_UP                 ::Makeshift::Key::Up
#define MK_KEY_PAGE_UP            ::Makeshift::Key::PageUp
#define MK_KEY_PAGE_DOWN          ::Makeshift::Key::PageDown
#define MK_KEY_HOME               ::Makeshift::Key::Home
#define MK_KEY_END                ::Makeshift::Key::End
#define MK_KEY_CAPS_LOCK          ::Makeshift::Key::CapsLock
#define MK_KEY_SCROLL_LOCK        ::Makeshift::Key::ScrollLock
#define MK_KEY_NUM_LOCK           ::Makeshift::Key::NumLock
#define MK_KEY_PRINT_SCREEN       ::Makeshift::Key::PrintScreen
#define MK_KEY_PAUSE              ::Makeshift::Key::Pause
#define MK_KEY_F1                 ::Makeshift::Key::F1
#define MK_KEY_F2                 ::Makeshift::Key::F2
#define MK_KEY_F3                 ::Makeshift::Key::F3
#define MK_KEY_F4                 ::Makeshift::Key::F4
#define MK_KEY_F5                 ::Makeshift::Key::F5
#define MK_KEY_F6                 ::Makeshift::Key::F6
#define MK_KEY_F7                 ::Makeshift::Key::F7
#define MK_KEY_F8                 ::Makeshift::Key::F8
#define MK_KEY_F9                 ::Makeshift::Key::F9
#define MK_KEY_F10                ::Makeshift::Key::F10
#define MK_KEY_F11                ::Makeshift::Key::F11
#define MK_KEY_F12                ::Makeshift::Key::F12
#define MK_KEY_F13                ::Makeshift::Key::F13
#define MK_KEY_F14                ::Makeshift::Key::F14
#define MK_KEY_F15                ::Makeshift::Key::F15
#define MK_KEY_F16                ::Makeshift::Key::F16
#define MK_KEY_F17                ::Makeshift::Key::F17
#define MK_KEY_F18                ::Makeshift::Key::F18
#define MK_KEY_F19                ::Makeshift::Key::F19
#define MK_KEY_F20                ::Makeshift::Key::F20
#define MK_KEY_F21                ::Makeshift::Key::F21
#define MK_KEY_F22                ::Makeshift::Key::F22
#define MK_KEY_F23                ::Makeshift::Key::F23
#define MK_KEY_F24                ::Makeshift::Key::F24
#define MK_KEY_F25                ::Makeshift::Key::F25
#define MK_KEY_KP_0               ::Makeshift::Key::KP0
#define MK_KEY_KP_1               ::Makeshift::Key::KP1
#define MK_KEY_KP_2               ::Makeshift::Key::KP2
#define MK_KEY_KP_3               ::Makeshift::Key::KP3
#define MK_KEY_KP_4               ::Makeshift::Key::KP4
#define MK_KEY_KP_5               ::Makeshift::Key::KP5
#define MK_KEY_KP_6               ::Makeshift::Key::KP6
#define MK_KEY_KP_7               ::Makeshift::Key::KP7
#define MK_KEY_KP_8               ::Makeshift::Key::KP8
#define MK_KEY_KP_9               ::Makeshift::Key::KP9
#define MK_KEY_KP_DECIMAL         ::Makeshift::Key::KPDecimal
#define MK_KEY_KP_DIVIDE          ::Makeshift::Key::KPDivide
#define MK_KEY_KP_MULTIPLY        ::Makeshift::Key::KPMultiply
#define MK_KEY_KP_SUBTRACT        ::Makeshift::Key::KPSubtract
#define MK_KEY_KP_ADD             ::Makeshift::Key::KPAdd
#define MK_KEY_KP_ENTER           ::Makeshift::Key::KPEnter
#define MK_KEY_KP_EQUAL           ::Makeshift::Key::KPEqual
#define MK_KEY_LEFT_SHIFT         ::Makeshift::Key::LeftShift
#define MK_KEY_LEFT_CONTROL       ::Makeshift::Key::LeftControl
#define MK_KEY_LEFT_ALT           ::Makeshift::Key::LeftAlt
#define MK_KEY_LEFT_SUPER         ::Makeshift::Key::LeftSuper
#define MK_KEY_RIGHT_SHIFT        ::Makeshift::Key::RightShift
#define MK_KEY_RIGHT_CONTROL      ::Makeshift::Key::RightControl
#define MK_KEY_RIGHT_ALT          ::Makeshift::Key::RightAlt
#define MK_KEY_RIGHT_SUPER        ::Makeshift::Key::RightSuper
#define MK_KEY_MENU               ::Makeshift::Key::Menu