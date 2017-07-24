//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "GLFWKeyboard.hpp"

#include <GLFW/glfw3.h>

namespace XYZ::Input::Keyboard::GLFW {

	static KeyboardEvent createEvent(int key, int scanCode, int modifierKeys, bool repeat = false) {
		return KeyboardEvent {
				static_cast<Key>(key),
				(
						(modifierKeys & GLFW_MOD_SHIFT ? ModifierKey::SHIFT : ModifierKey::NONE) |
						(modifierKeys & GLFW_MOD_CONTROL ? ModifierKey::CONTROL : ModifierKey::NONE) |
						(modifierKeys & GLFW_MOD_ALT ? ModifierKey::ALT : ModifierKey::NONE) |
						(modifierKeys & GLFW_MOD_SUPER ? ModifierKey::COMMAND : ModifierKey::NONE)
				)
		};
	}

	void GLFWKeyboard::notifyKeyPress(int key, int scanCode, int modifierKeys) {
		if(delegate == nullptr) {
			return;
		}
		delegate->keyPress(*this, createEvent(key, scanCode, modifierKeys));
	}

	void GLFWKeyboard::notifyKeyRelease(int key, int scanCode, int modifierKeys) {
		if(delegate == nullptr) {
			return;
		}
		delegate->keyRelease(*this, createEvent(key, scanCode, modifierKeys));
	}

	void GLFWKeyboard::notifyKeyRepeat(int key, int scanCode, int modifierKeys) {
		if(delegate == nullptr) {
			return;
		}
		delegate->keyPress(*this, createEvent(key, scanCode, modifierKeys, true));
	}

}
