//
// Created by Rogiel Sulzbach on 10/26/17.
//

import Foundation
import GameEngine
import CGLFW3

public class GLFWMouse: Mouse {

    /// The mouse delegate
    /// This delegate will be called with updates on mouse movement,
    /// scrolling and clicking events
    public weak var delegate: MouseDelegate? = nil

    /// The mouse cursor shape
    public var cursorShape: MouseCursorShape = .arrow

    /// The currently pressed moused buttons
    public var buttons: [MouseButton] = []

    /// The current mouse position
    public var position: Mouse.Position = Mouse.Position()

    /// A flag indicating if the cursor is visible or not
    public var visible: Bool {
        get { return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL }
        set {
            if self.delegate?.mouseShouldHideCursor(self) ?? true {
                self.delegate?.mouseWillHideCursor(self)
                glfwSetInputMode(window, GLFW_CURSOR, newValue ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED)
                self.delegate?.mouseDidHideCursor(self)
            }
        }
    }

    // A opaque pointer that represents the GLFW window
    private var window: OpaquePointer

    /// Creates a new GLFW mouse
    public init(window: OpaquePointer) {
        self.window = window

        glfwSetCursorPosCallback(window) { (window, xpos, ypos) in
            if let context = GLFW.current {
                context.mouse.position = Mouse.Position(x: xpos, y: ypos)
                context.mouse.delegate?.mouse(context.mouse, didMoveToLocation: context.mouse.position)
            }
        }
        glfwSetScrollCallback(window) { (window, xpos, ypos) in
            if let context = GLFW.current {
                if xpos != 0 {
                    context.mouse.delegate?.mouse(context.mouse, scrollBy: xpos, direction: .horizontal)
                }
                if ypos != 0 {
                    context.mouse.delegate?.mouse(context.mouse, scrollBy: ypos, direction: .vertical)
                }
            }
        }
        glfwSetMouseButtonCallback(window) { (window, button, action, mods) in
            if let context = GLFW.current {
                let mouse = context.mouse!
                let button = mouse.mouseButton(button)

                if action == GLFW_PRESS {
                    if !mouse.buttons.contains(button) {
                        mouse.buttons.append(button)
                    }
                    context.mouse.delegate?.mouse(context.mouse, pressedButton: button)
                } else if action == GLFW_RELEASE {
                    mouse.buttons = mouse.buttons.filter { $0 != button }
                    context.mouse.delegate?.mouse(context.mouse, releasedButton: button)
                }
            }
        }
    }

    deinit {
        glfwSetCursorPosCallback(window, nil)
        glfwSetScrollCallback(window, nil)
        glfwSetMouseButtonCallback(window, nil)
    }

    fileprivate func mouseButton(_ button: Int32) -> MouseButton {
        switch button {
        case GLFW_MOUSE_BUTTON_LEFT: return .left
        case GLFW_MOUSE_BUTTON_RIGHT: return .right
        case GLFW_MOUSE_BUTTON_MIDDLE: return .middle
        default: return .button8
        }
    }

}
