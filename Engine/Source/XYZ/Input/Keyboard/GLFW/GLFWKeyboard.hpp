//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Input/Keyboard/Keyboard.hpp"

namespace XYZ::Input::Keyboard::GLFW {

	class GLFWKeyboard : public Keyboard {
		friend class GLFWKeyboardController;


	private:
		void notifyKeyPress(int key, int scanCode, int modifierKeys);
		void notifyKeyRelease(int key, int scanCode, int modifierKeys);
		void notifyKeyRepeat(int key, int scanCode, int modifierKeys);

	};

}

