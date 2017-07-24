//
// Created by Rogiel Sulzbach on 7/22/17.
//

#include "GLFWWindow.hpp"

namespace XYZ::Graphics::Window::GLFW {

	GLFWWindow::GLFWWindow(unsigned int width, unsigned int height, const std::string& title) {
// Initialise GLFW
		if(glfwInit() == GLFW_FALSE) {
			return;
		}

//		glfwWindowHint(GLFW_SAMPLES, 0);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		// Open a window and create its OpenGL context
		window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		if(window == nullptr) {
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);

		// Initialize GLEW
		glewExperimental = GL_TRUE; // Needed for core profile
		if(glewInit() != GLEW_OK) {
			glfwTerminate();
			return;
		}
	}

	// -----------------------------------------------------------------------------------------------------------------

	void GLFWWindow::activate() {
		glfwMakeContextCurrent(window);
	}

	void GLFWWindow::deactivate() {
	}

	// -----------------------------------------------------------------------------------------------------------------

	void GLFWWindow::setTitle(const std::string& title) {
		glfwSetWindowTitle(window, title.data());
	}

	void GLFWWindow::setSize(unsigned int width, unsigned int height) {
		glfwSetWindowSize(window, width, height);
	}

	void GLFWWindow::setFullscreen(bool fullscreen) {
		auto monitor = glfwGetPrimaryMonitor();

		int width;
		int height;
		glfwGetMonitorPhysicalSize(monitor, &width, &height);

		if(fullscreen) {
			glfwSetWindowMonitor(window, monitor, 0, 0, width, height, GLFW_DONT_CARE);
		} else {
			int xpos;
			int ypos;

			glfwGetMonitorPos(monitor, &xpos, &ypos);
			glfwSetWindowMonitor(window, monitor, xpos, ypos, width, height, GLFW_DONT_CARE);
		}
	}

}