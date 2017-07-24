//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Input/Keyboard/KeyboardController.hpp"

#include "XYZ/Input/Keyboard/GLFW/GLFWKeyboard.hpp"

#include <glfw/glfw3.h>

namespace XYZ::Input::Keyboard::GLFW {

	class GLFWKeyboardController : public KeyboardController {
	private:
		/**
		 * The GLFW window to capture keyboard events for
		 */
		GLFWwindow* window;

		/**
		 * The systems primary keyboard
		 */
		std::shared_ptr<GLFWKeyboard> primaryKeyboard;

	public:
		/**
		 * Create a new keyboard controller for the given <tt>window</tt>.
		 *
		 * @param window the GLFW window to capture keyboard events for
		 */
		explicit GLFWKeyboardController(GLFWwindow* window);

		/**
		 * Destroys the keyboard controller
		 */
		virtual ~GLFWKeyboardController();

	public:
		/**
		 * @return the systems primary keyboard
		 */
		std::shared_ptr<Keyboard> getPrimaryKeyboard() override;

	private:
		/**
		 * Handles the keyboard event
		 *
		 * @param key The keyboard key that was pressed or released.
		 * @param scancode The system-specific scancode of the key.
		 * @param action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.
		 * @param modifierKeys Bit field describing which modifier keys were
		 *  held down.
		 */
		void handleKeyboardEvent(int key, int scanCode, int action, int modifierKeys);

	};

}

