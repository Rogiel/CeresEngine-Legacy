//
// Created by Rogiel Sulzbach on 7/13/17.
//

#include "GLFWMouseController.hpp"

namespace XYZ::Input::Mouse::GLFW {

	static GLFWMouseController* activeController;

	GLFWMouseController::GLFWMouseController(GLFWwindow* window) :
			window(window), primaryMouse(std::make_shared<GLFWMouse>()) {
		activeController = this;
		glfwSetCursorPosCallback(window, [](GLFWwindow*, double x, double y) {
			if(activeController == nullptr) {
				return;
			}
			activeController->handleMouseMoveEvent(x, y);
		});
		glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int modifierKeys) {
			if(activeController == nullptr) {
				return;
			}
			activeController->handleMouseButtonsEvent(button, action, modifierKeys);
		});
		glfwSetScrollCallback(window, [](GLFWwindow*, double x, double y) {
			if(activeController == nullptr) {
				return;
			}
			activeController->handleMouseScrollEvent(x, y);
		});
	}

	GLFWMouseController::~GLFWMouseController() {
		glfwSetCursorPosCallback(window, nullptr);
		glfwSetMouseButtonCallback(window, nullptr);
		activeController = nullptr;
	}

	// -----------------------------------------------------------------------------------------------------------------

	std::shared_ptr<Mouse> GLFWMouseController::getPrimaryMouse() {
		return primaryMouse;
	}

	// -----------------------------------------------------------------------------------------------------------------

	void GLFWMouseController::handleMouseMoveEvent(double x, double y) {
		primaryMouse->notifyMouseMoveEvent(x, y);
	}

	void GLFWMouseController::handleMouseButtonsEvent(int button, int action, int modifierKeys) {
		primaryMouse->notifyMouseButtonsEvent(button, action, modifierKeys);
	}

	void GLFWMouseController::handleMouseScrollEvent(double x, double y) {
		primaryMouse->notifyMouseScrollEvent(x, y);
	}

}