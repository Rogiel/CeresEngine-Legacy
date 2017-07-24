//
// Created by Rogiel Sulzbach on 7/13/17.
//

#pragma once

#include "XYZ/Input/Mouse/Mouse.hpp"

namespace XYZ::Input::Mouse::GLFW {

	class GLFWMouse : public Mouse {
		friend class GLFWMouseController;

	private:
		/**
		 * @param x The scroll offset along the x-axis.
 		 * @param y The scroll offset along the y-axis.
		 */
		void notifyMouseMoveEvent(double x, double y);

		/**
		 * Handles a mouse click event
		 *
		 * @param button The mouse button that was pressed or released.
		 * @param action One of `GLFW_PRESS` or `GLFW_RELEASE`.
		 * @param mods Bit field describing which modifier keys were held down.
		 */
		void notifyMouseButtonsEvent(int button, int action, int modifierKeys);

		/**
		 * @param x The scroll offset along the x-axis.
 		 * @param y The scroll offset along the y-axis.
		 */
		void notifyMouseScrollEvent(double x, double y);
	};

}

