//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "InputDeviceManager.hpp"

namespace XYZ::Input {

	InputDeviceManager::InputDeviceManager(const std::shared_ptr<Keyboard::KeyboardController>& keyboardController,
										   const std::shared_ptr<Mouse::MouseController>& mouseController)
			: keyboardController(keyboardController), mouseController(mouseController) {}

	// -----------------------------------------------------------------------------------------------------------------

	std::shared_ptr<Keyboard::Keyboard> InputDeviceManager::getPrimaryKeyboard() {
		if(keyboardController == nullptr) {
			return nullptr;
		}
		return keyboardController->getPrimaryKeyboard();
	}

	std::shared_ptr<Mouse::Mouse> InputDeviceManager::getPrimaryMouse() {
		if(mouseController == nullptr) {
			return nullptr;
		}
		return mouseController->getPrimaryMouse();
	}

	// -----------------------------------------------------------------------------------------------------------------

	const std::shared_ptr<Keyboard::KeyboardController>& InputDeviceManager::getKeyboardController() const {
		return keyboardController;
	}

	void InputDeviceManager::setKeyboardController(
			const std::shared_ptr<Keyboard::KeyboardController>& keyboardController) {
		InputDeviceManager::keyboardController = keyboardController;
	}

	const std::shared_ptr<Mouse::MouseController>& InputDeviceManager::getMouseController() const {
		return mouseController;
	}

	void InputDeviceManager::setMouseController(const std::shared_ptr<Mouse::MouseController>& mouseController) {
		InputDeviceManager::mouseController = mouseController;
	}

}