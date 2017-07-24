//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Input/Keyboard/KeyboardController.hpp"
#include "XYZ/Input/Mouse/MouseController.hpp"

namespace XYZ::Input {

	class InputDeviceManager {
	private:
		/**
		 * The keyboard controller
		 */
		std::shared_ptr<Keyboard::KeyboardController> keyboardController;

		/**
		 * The mouse controller
		 */
		std::shared_ptr<Mouse::MouseController> mouseController;

	public:
		/**
		 * Create a new input device manager
		 *
		 * @param keyboardController the keyboard controller
		 * @param mouseController the mouse controller
		 */
		explicit InputDeviceManager(const std::shared_ptr<Keyboard::KeyboardController>& keyboardController = nullptr,
									const std::shared_ptr<Mouse::MouseController>& mouseController = nullptr);

	public:
		/**
		 * @return the systems primary keyboard
		 */
		std::shared_ptr<Keyboard::Keyboard> getPrimaryKeyboard();

		/**
		 * @return the systems primary mouse
		 */
		std::shared_ptr<Mouse::Mouse> getPrimaryMouse();

	public:
		/**
		 * @return the keyboard controller
		 */
		const std::shared_ptr<Keyboard::KeyboardController>& getKeyboardController() const;

		/**
		 * @param keyboardController the keyboard controller
		 */
		void setKeyboardController(const std::shared_ptr<Keyboard::KeyboardController>& keyboardController);

		/**
		 * @return the mouse controller
		 */
		const std::shared_ptr<Mouse::MouseController>& getMouseController() const;

		/**
		 * @param mouseController the mouse controller
		 */
		void setMouseController(const std::shared_ptr<Mouse::MouseController>& mouseController);

	};

}

