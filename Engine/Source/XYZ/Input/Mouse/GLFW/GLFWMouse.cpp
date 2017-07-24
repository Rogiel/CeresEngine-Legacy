//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "GLFWMouse.hpp"

#include <glfw/glfw3.h>

namespace XYZ::Input::Mouse::GLFW {

	void GLFWMouse::notifyMouseMoveEvent(double x, double y) {
		if(delegate == nullptr) {
			return;
		}
		delegate->mouseMove(*this, MouseMoveEvent{x, y});
	}

	void GLFWMouse::notifyMouseButtonsEvent(int button, int action, int modifierKeys) {
		if(delegate == nullptr) {
			return;
		}

		MouseClickEvent event;
		event.button = static_cast<Button>(button);
		event.modifiers = (
				(modifierKeys & GLFW_MOD_SHIFT ? ModifierKey::SHIFT : ModifierKey::NONE) |
				(modifierKeys & GLFW_MOD_CONTROL ? ModifierKey::CONTROL : ModifierKey::NONE) |
				(modifierKeys & GLFW_MOD_ALT ? ModifierKey::ALT : ModifierKey::NONE) |
				(modifierKeys & GLFW_MOD_SUPER ? ModifierKey::COMMAND : ModifierKey::NONE)
		);

		if(action == GLFW_PRESS) {
			delegate->mouseClick(*this, event);
		} else if(action == GLFW_RELEASE) {
			delegate->mouseRelease(*this, event);
		}
	}

	void GLFWMouse::notifyMouseScrollEvent(double x, double y) {
		if(delegate == nullptr) {
			return;
		}
		delegate->mouseScroll(*this, MouseScrollEvent{x, y});
	}

}
