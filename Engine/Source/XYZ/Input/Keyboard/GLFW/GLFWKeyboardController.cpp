//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "GLFWKeyboardController.hpp"

namespace XYZ::Input::Keyboard::GLFW {

	static GLFWKeyboardController* currentController;

	GLFWKeyboardController::GLFWKeyboardController(GLFWwindow* window) :
			window(window), primaryKeyboard(std::make_shared<GLFWKeyboard>()) {
		currentController = this;
		glfwSetKeyCallback(window, [](GLFWwindow*, int key, int scanCode, int action, int modifierKeys) {
			if(currentController == nullptr) {
				return;
			}
			currentController->handleKeyboardEvent(key, scanCode, action, modifierKeys);
		});
	}

	GLFWKeyboardController::~GLFWKeyboardController() {
		glfwSetKeyCallback(window, nullptr);
		currentController = nullptr;
	}

	std::shared_ptr<Keyboard> GLFWKeyboardController::getPrimaryKeyboard() {
		return primaryKeyboard;
	}

	void GLFWKeyboardController::handleKeyboardEvent(int key, int scanCode, int action, int modifierKeys) {
		switch(action) {
			case GLFW_PRESS:
				primaryKeyboard->notifyKeyPress(key, scanCode, modifierKeys);
				break;

			case GLFW_RELEASE:
				primaryKeyboard->notifyKeyRelease(key, scanCode, modifierKeys);
				break;

			case GLFW_REPEAT:
				primaryKeyboard->notifyKeyRepeat(key, scanCode, modifierKeys);
				break;
		}
	}


}