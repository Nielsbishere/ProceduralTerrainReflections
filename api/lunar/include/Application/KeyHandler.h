#pragma once
#include "GL/glew.h"
#include <GLFW\glfw3.h>
namespace osomi {
	enum EMouseButton {
		M_LEFT, M_RIGHT, M_MIDDLE, M4, M5, M6, M7, M8, M9
	};
	enum EKeyButton {
		SPACE = (0 << 16) | GLFW_KEY_SPACE,
		APOSTROPHE = (1 << 16) | GLFW_KEY_APOSTROPHE,
		COMMA = (2 << 16) | GLFW_KEY_COMMA,
		MINUS = (3 << 16) | GLFW_KEY_MINUS,
		PERIOD = (4 << 16) | GLFW_KEY_PERIOD,
		SLASH = (5 << 16) | GLFW_KEY_SLASH,
		N0 = (6 << 16) | GLFW_KEY_0,
		N1 = (7 << 16) | GLFW_KEY_1,
		N2 = (8 << 16) | GLFW_KEY_2,
		N3 = (9 << 16) | GLFW_KEY_3,
		N4 = (10 << 16) | GLFW_KEY_4,
		N5 = (11 << 16) | GLFW_KEY_5,
		N6 = (12 << 16) | GLFW_KEY_6,
		N7 = (13 << 16) | GLFW_KEY_7,
		N8 = (14 << 16) | GLFW_KEY_8,
		N9 = (15 << 16) | GLFW_KEY_9,
		SEMICOLON = (16 << 16) | GLFW_KEY_SEMICOLON,
		EQUALS = (17 << 16) | GLFW_KEY_EQUAL,
		A = (18 << 16) | GLFW_KEY_A,
		B = (19 << 16) | GLFW_KEY_B,
		C = (20 << 16) | GLFW_KEY_C,
		D = (21 << 16) | GLFW_KEY_D,
		E = (22 << 16) | GLFW_KEY_E,
		F = (23 << 16) | GLFW_KEY_F,
		G = (24 << 16) | GLFW_KEY_G,
		H = (25 << 16) | GLFW_KEY_H,
		I = (26 << 16) | GLFW_KEY_I,
		J = (27 << 16) | GLFW_KEY_J,
		K = (28 << 16) | GLFW_KEY_K,
		L = (29 << 16) | GLFW_KEY_L,
		M = (30 << 16) | GLFW_KEY_M,
		N = (31 << 16) | GLFW_KEY_N,
		O = (32 << 16) | GLFW_KEY_O,
		P = (33 << 16) | GLFW_KEY_P,
		Q = (34 << 16) | GLFW_KEY_Q,
		R = (35 << 16) | GLFW_KEY_R,
		S = (36 << 16) | GLFW_KEY_S,
		T = (37 << 16) | GLFW_KEY_T,
		U = (38 << 16) | GLFW_KEY_U,
		V = (39 << 16) | GLFW_KEY_V,
		W = (40 << 16) | GLFW_KEY_W,
		X = (41 << 16) | GLFW_KEY_X,
		Y = (42 << 16) | GLFW_KEY_Y,
		Z = (43 << 16) | GLFW_KEY_Z,
		LEFT_BRACKET = (44 << 16) | GLFW_KEY_LEFT_BRACKET,
		BACKSLASH = (45 << 16) | GLFW_KEY_BACKSLASH,
		RIGHT_BRACKET = (46 << 16) | GLFW_KEY_RIGHT_BRACKET,
		GRAVE_ACCENT = (47 << 16) | GLFW_KEY_GRAVE_ACCENT,
		WORLD_1 = (48 << 16) | GLFW_KEY_WORLD_1,
		WORLD_2 = (49 << 16) | GLFW_KEY_WORLD_2,
		ESCAPE = (50 << 16) | GLFW_KEY_ESCAPE,
		ENTER = (51 << 16) | GLFW_KEY_ENTER,
		TAB = (52 << 16) | GLFW_KEY_TAB,
		BACKSPACE = (53 << 16) | GLFW_KEY_BACKSPACE,
		INSERT = (54 << 16) | GLFW_KEY_INSERT,
		DELETE = (55 << 16) | GLFW_KEY_DELETE,
		RIGHT = (56 << 16) | GLFW_KEY_RIGHT,
		LEFT = (57 << 16) | GLFW_KEY_LEFT,
		DOWN = (58 << 16) | GLFW_KEY_DOWN,
		UP = (59 << 16) | GLFW_KEY_UP,
		PAGE_UP = (60 << 16) | GLFW_KEY_PAGE_UP,
		PAGE_DOWN = (61 << 16) | GLFW_KEY_PAGE_DOWN,
		HOME = (62 << 16) | GLFW_KEY_HOME,
		END = (63 << 16) | GLFW_KEY_END,
		CAPS_LOCK = (64 << 16) | GLFW_KEY_CAPS_LOCK,
		SCROLL_LOCK = (65 << 16) | GLFW_KEY_PAGE_DOWN,
		NUM_LOCK = (66 << 16) | GLFW_KEY_NUM_LOCK,
		PRINT_SCREEN = (67 << 16) | GLFW_KEY_PRINT_SCREEN,
		PAUSE = (68 << 16) | GLFW_KEY_PAUSE,
		F1 = (69 << 16) | GLFW_KEY_F1,
		F2 = (70 << 16) | GLFW_KEY_F2,
		F3 = (71 << 16) | GLFW_KEY_F3,
		F4 = (72 << 16) | GLFW_KEY_F4,
		F5 = (73 << 16) | GLFW_KEY_F5,
		F6 = (74 << 16) | GLFW_KEY_F6,
		F7 = (75 << 16) | GLFW_KEY_F7,
		F8 = (76 << 16) | GLFW_KEY_F8,
		F9 = (77 << 16) | GLFW_KEY_F9,
		F10 = (78 << 16) | GLFW_KEY_F10,
		F11 = (79 << 16) | GLFW_KEY_F11,
		F12 = (80 << 16) | GLFW_KEY_F12,
		F13 = (81 << 16) | GLFW_KEY_F13,
		F14 = (82 << 16) | GLFW_KEY_F14,
		F15 = (83 << 16) | GLFW_KEY_F15,
		F16 = (84 << 16) | GLFW_KEY_F16,
		F17 = (85 << 16) | GLFW_KEY_F17,
		F18 = (86 << 16) | GLFW_KEY_F18,
		F19 = (87 << 16) | GLFW_KEY_F19,
		F20 = (88 << 16) | GLFW_KEY_F20,
		F21 = (89 << 16) | GLFW_KEY_F21,
		F22 = (90 << 16) | GLFW_KEY_F22,
		F23 = (91 << 16) | GLFW_KEY_F23,
		F24 = (92 << 16) | GLFW_KEY_F24,
		F25 = (93 << 16) | GLFW_KEY_F25,
		KP0 = (94 << 16) | GLFW_KEY_KP_0,
		KP1 = (95 << 16) | GLFW_KEY_KP_1,
		KP2 = (96 << 16) | GLFW_KEY_KP_2,
		KP3 = (97 << 16) | GLFW_KEY_KP_3,
		KP4 = (98 << 16) | GLFW_KEY_KP_4,
		KP5 = (99 << 16) | GLFW_KEY_KP_5,
		KP6 = (100 << 16) | GLFW_KEY_KP_6,
		KP7 = (101 << 16) | GLFW_KEY_KP_7,
		KP8 = (102 << 16) | GLFW_KEY_KP_8,
		KP9 = (103 << 16) | GLFW_KEY_KP_9,
		KP_DECIMAL = (104 << 16) | GLFW_KEY_KP_0,
		KP_DIVIDE = (105 << 16) | GLFW_KEY_KP_1,
		KP_MULTIPLY = (106 << 16) | GLFW_KEY_KP_2,
		KP_SUBTRACT = (107 << 16) | GLFW_KEY_KP_3,
		KP_ADD = (108 << 16) | GLFW_KEY_KP_4,
		KP_ENTER = (109 << 16) | GLFW_KEY_KP_5,
		KP_EQUAL = (110 << 16) | GLFW_KEY_KP_6,
		LEFT_SHIFT = (111 << 16) | GLFW_KEY_KP_7,
		LEFT_CONTROL = (112 << 16) | GLFW_KEY_KP_8,
		LEFT_ALT = (113 << 16) | GLFW_KEY_KP_9,
		LEFT_SUPER = (114 << 16) | GLFW_KEY_KP_9,
		RIGHT_SHIFT = (115 << 16) | GLFW_KEY_KP_7,
		RIGHT_CONTROL = (116 << 16) | GLFW_KEY_KP_8,
		RIGHT_ALT = (117 << 16) | GLFW_KEY_KP_9,
		RIGHT_SUPER = (118 << 16) | GLFW_KEY_KP_9,
		MENU = (119 << 16) | GLFW_KEY_MENU,

		LAST = MENU,
		LENGTH = ((LAST & 0xFFFF0000) >> 16) + 1
	};
	class MouseEvent {
	private:
		EMouseButton clicked;
		bool down;
		bool shiftDown, controlDown, altDown, superDown;
		bool valid;
	public:
		MouseEvent(EMouseButton _clicked, bool _down, bool _shift, bool _ctrl, bool _alt, bool _super, bool _valid = true) : clicked(_clicked), down(_down), shiftDown(_shift), controlDown(_ctrl), altDown(_alt), superDown(_super), valid(_valid) {}
		MouseEvent() : MouseEvent(M9, false, false, false, false, false, false) {}
		EMouseButton getClicked() { return clicked; }
		bool isDown() { return down; }
		bool isShiftDown() { return shiftDown; }
		bool isControlDown() { return controlDown; }
		bool isAltDown() { return altDown; }
		bool isSuperDown() { return superDown; }
		bool isValid() { return valid; }
	};
	class KeyEvent {
	private:
		EKeyButton pressed;
		bool down;
		bool shiftDown, controlDown, altDown, superDown;
		bool valid;
	public:
		KeyEvent(EKeyButton _pressed, bool _down, bool _shift, bool _ctrl, bool _alt, bool _super, bool _valid = true) : pressed(_pressed), down(_down), shiftDown(_shift), controlDown(_ctrl), altDown(_alt), superDown(_super), valid(_valid) {}
		KeyEvent() : KeyEvent(MENU, false, false, false, false, false, false) {}
		EKeyButton getPressed() { return pressed; }
		bool isDown() { return down; }
		bool isShiftDown() { return shiftDown; }
		bool isControlDown() { return controlDown; }
		bool isAltDown() { return altDown; }
		bool isSuperDown() { return superDown; }
		bool isValid() { return valid; }
	};
	class KeyHandler {
	public:
		KeyHandler();
		bool isPressed(EMouseButton id);
		bool isPressed(EKeyButton id);
		bool isReleased(EMouseButton id);
		bool isReleased(EKeyButton id);
		bool isDown(EMouseButton id);
		bool isDown(EKeyButton id);
		void update();
		MouseEvent mouseButton(int button, int action, int mods);
		KeyEvent keyButton(int button, int scancode, int action, int mods);
	private:
		bool prevMouse[9];
		bool mouse[9];
		bool prevKey[EKeyButton::LENGTH];
		bool key[EKeyButton::LENGTH];
		static EKeyButton keys[EKeyButton::LENGTH];
	};
}