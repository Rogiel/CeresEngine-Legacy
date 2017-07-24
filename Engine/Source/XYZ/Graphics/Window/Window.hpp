//
// Created by Rogiel Sulzbach on 7/22/17.
//

#pragma once

#include <string>

namespace XYZ::Graphics::Window {

	class Window {
	public:
		/**
		 * Activates the window
		 */
		virtual void activate() = 0;

		/**
		 * Deactivates the window
		 */
		virtual void deactivate() = 0;

	public:
		/**
		 * Sets the window title
		 *
		 * @param title the window title
		 */
		virtual void setTitle(const std::string& title) = 0;

		/**
		 * Resizes the window
		 *
		 * @param width the window's width
		 * @param height the window's height
		 */
		virtual void setSize(unsigned int width, unsigned int height) = 0;

		/**
		 * Sets the window to full screen
		 *
		 * @param fullscreen true to make the window fullscreen, false otherwise
		 */
		virtual void setFullscreen(bool fullscreen) = 0;

	};

}



