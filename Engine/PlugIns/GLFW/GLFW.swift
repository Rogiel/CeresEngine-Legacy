//
// Created by Rogiel Sulzbach on 10/26/17.
//

import Foundation

import SGLOpenGL
import CGLFW3

public class GLFW {

    public let window: OpaquePointer!
    public var mouse: GLFWMouse!
    public var keyboard: GLFWKeyboard!

    public init?(width: UInt, height: UInt) {
        glfwInit()

        // Set all the required options for GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
        glfwWindowHint(GLFW_SAMPLES, 4)
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE)
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE)

        window = glfwCreateWindow(GLsizei(width), GLsizei(height), "Rogiel Swift Game Engine", nil, nil)
        guard window != nil else {
            print("Failed to create GLFW window")
            return nil
        }
        glfwMakeContextCurrent(window)
        glfwSetWindowUserPointer(window, Unmanaged<GLFW>.passUnretained(self).toOpaque())

        self.mouse = GLFWMouse(window: window)
        self.keyboard = GLFWKeyboard(window: window)

        glViewport(x: 0, y: 0, width: GLsizei(width), height: GLsizei(height))
    }

    deinit {
        glfwTerminate()
    }

    public func close() {
        glfwSetWindowShouldClose(window, GLFW_TRUE)
    }

    internal static var current: GLFW? {
        let userPointer = glfwGetWindowUserPointer(glfwGetCurrentContext())
        if userPointer == nil {
            return nil
        }
        return Unmanaged<GLFW>.fromOpaque(userPointer!).takeUnretainedValue()
    }

}
