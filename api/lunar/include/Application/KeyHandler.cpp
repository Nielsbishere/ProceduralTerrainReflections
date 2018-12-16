#include "KeyHandler.h"

using namespace osomi;

EKeyButton KeyHandler::keys[EKeyButton::LENGTH] = {
	SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH, N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, SEMICOLON, EQUALS, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET, GRAVE_ACCENT, WORLD_1, WORLD_2, ESCAPE, ENTER, TAB, BACKSPACE, INSERT, DELETE, RIGHT, LEFT, DOWN, UP, PAGE_UP, PAGE_DOWN, HOME, END, CAPS_LOCK, SCROLL_LOCK,
	NUM_LOCK, PRINT_SCREEN, PAUSE, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25, KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9,
	KP_DECIMAL, KP_DIVIDE, KP_MULTIPLY, KP_SUBTRACT, KP_ADD, KP_ENTER, KP_EQUAL, LEFT_SHIFT, LEFT_CONTROL, LEFT_ALT, LEFT_SUPER, RIGHT_SHIFT, RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER, MENU
};
MouseEvent KeyHandler::mouseButton(int button, int action, int mods) {
	if (button > EMouseButton::M9 || (action != GLFW_PRESS && action != GLFW_RELEASE) || mods > 0b1111)return MouseEvent();
	mouse[button] = action == GLFW_PRESS;
	return MouseEvent((EMouseButton)button, action == GLFW_PRESS, mods & 0x1, mods & 0x2, mods & 0x4, mods & 0x8);
}
KeyEvent KeyHandler::keyButton(int key, int scancode, int action, int mods) {
	if (key > EKeyButton::MENU || (action != GLFW_PRESS && action != GLFW_RELEASE && action != GLFW_REPEAT) || mods > 0b1111)return KeyEvent();
	bool found = false;
	EKeyButton button;
	for (unsigned int i = 0; i < EKeyButton::LENGTH; i++) {
		if ((keys[i] & 0xFFFF) == key) {
			found = true;
			button = keys[i];
			break;
		}
	}
	if (!found)return KeyEvent();
	this->key[(button & 0xFFFF0000) >> 16] = action != GLFW_RELEASE;
	return KeyEvent(button, action != GLFW_RELEASE, mods & 0x1, mods & 0x2, mods & 0x4, mods & 0x8);
}
void KeyHandler::update() {
	for (unsigned int i = 0; i < M9 + 1; i++)
		prevMouse[i] = mouse[i];
	for (unsigned int i = 0; i < EKeyButton::LENGTH; i++)
		prevKey[i] = key[i];
}
KeyHandler::KeyHandler() {
	for (unsigned int i = 0; i < M9 + 1; i++)
		prevMouse[i] = mouse[i] = 0;
	for (unsigned int i = 0; i < EKeyButton::LENGTH; i++)
		prevKey[i] = key[i] = 0;
}
bool KeyHandler::isPressed(EMouseButton id) {
	return mouse[id] && !prevMouse[id];
}
bool KeyHandler::isPressed(EKeyButton ide) {
	unsigned int id = (ide & 0xFFFF0000) >> 16;
	return key[id] && !prevKey[id];
}
bool KeyHandler::isReleased(EMouseButton id) {
	return !mouse[id] && prevMouse[id];
}
bool KeyHandler::isReleased(EKeyButton ide) {
	unsigned int id = (ide & 0xFFFF0000) >> 16;
	return !key[id] && prevKey[id];
}
bool KeyHandler::isDown(EMouseButton id) {
	return mouse[id] && prevMouse[id];
}
bool KeyHandler::isDown(EKeyButton ide) {
	unsigned int id = (ide & 0xFFFF0000) >> 16;
	return key[id] && prevKey[id];
}