//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Input/Mouse/MouseController.hpp"

#include "XYZ/Input/Mouse/GLFW/GLFWMouse.hpp"
#include <glfw/glfw3.h>

namespace XYZ::Input::Mouse::GLFW {

	class GLFWMouseController : public MouseController {
	private:
		/**
		 * The GLFW window to capture mouse events for
		 */
		GLFWwindow* window;

		/**
		 * The systems primary mouse
		 */
		std::shared_ptr<GLFWMouse> primaryMouse;

	public:
		/**
		 * Create a new mouse controller for the given <tt>window</tt>.
		 *
		 * @param window the GLFW window to capture mouse events for
		 */
		explicit GLFWMouseController(GLFWwindow* window);

		/**
		 * Destroys the keyboard controller
		 */
		virtual ~GLFWMouseController();

	public:
		/**
		 * @return the systems primary mouse
		 */
		std::shared_ptr<Mouse> getPrimaryMouse() override;

	private:
		/**
		 * @param x The scroll offset along the x-axis.
 		 * @param y The scroll offset along the y-axis.
		 */
		void handleMouseMoveEvent(double x, double y);

		/**
		 * Handles a mouse click event
		 *
		 * @param button The mouse button that was pressed or released.
		 * @param action One of `GLFW_PRESS` or `GLFW_RELEASE`.
		 * @param mods Bit field describing which modifier keys were held down.
		 */
		void handleMouseButtonsEvent(int button, int action, int modifierKeys);

		/**
		 * @param x The scroll offset along the x-axis.
 		 * @param y The scroll offset along the y-axis.
		 */
		void handleMouseScrollEvent(double x, double y);

	};

}

