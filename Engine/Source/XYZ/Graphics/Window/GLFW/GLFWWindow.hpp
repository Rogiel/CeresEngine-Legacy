//
// Created by Rogiel Sulzbach on 7/22/17.
//

#pragma once

#include "XYZ/Graphics/Window/Window.hpp"

#include <GL/glew.h>
#include <glfw/glfw3.h>

namespace XYZ::Graphics::Window::GLFW {

	class GLFWWindow : public Window {
	private:
		GLFWwindow* window;

	public:
		GLFWWindow(unsigned int width = 1024, unsigned int height = 768, const std::string& title = "");

		/**
		 * Activates the window
		 */
		void activate() override;

		/**
		 * Deactivates the window
		 */
		void deactivate() override;

	public:
		/**
		 * Sets the window title
		 *
		 * @param title the window title
		 */
		void setTitle(const std::string& title) override;

		/**
		 * Resizes the window
		 *
		 * @param width the window's width
		 * @param height the window's height
		 */
		void setSize(unsigned int width, unsigned int height) override;

		/**
		 * Sets the window to full screen
		 *
		 * @param fullscreen true to make the window fullscreen, false otherwise
		 */
		void setFullscreen(bool fullscreen) override;

	};

}

