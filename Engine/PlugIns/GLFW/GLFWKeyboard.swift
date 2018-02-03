//
// Created by Rogiel Sulzbach on 10/26/17.
//

import Foundation
import GameEngine
import CGLFW3

public class GLFWKeyboard: Keyboard {

    /// The keyboard delegate
    public weak var delegate: KeyboardDelegate? = nil

    /// The currently pressed keyboard keys
    public var keys: [KeyboardKey] = []

    /// The current keyboard modifier keys
    public var modifiers: KeyboardModifier = KeyboardModifier()

    // A opaque pointer that represents the GLFW window
    private let window: OpaquePointer

    /// Creates a new GLFW keyboard
    public init(window: OpaquePointer) {
        self.window = window
        glfwSetKeyCallback(window) { (window, glfwKey, scancode, action, modes) in
            if let context = GLFW.current {
                let keyboard = context.keyboard!
                let key = keyboard.key(glfwKey)
                let modifiers = keyboard.modifiers(modes)

                if action == GLFW_PRESS {
                    keyboard.keys.append(key)
                    keyboard.modifiers = modifiers
                    keyboard.delegate?.keyboard(keyboard, pressKey: key, modifiers: modifiers, repeat: false)
                } else if action == GLFW_REPEAT {
                    keyboard.modifiers = modifiers
                    keyboard.delegate?.keyboard(keyboard, pressKey: key, modifiers: modifiers, repeat: true)
                } else if action == GLFW_RELEASE {
                    keyboard.keys = keyboard.keys.filter {
                        $0 != key
                    }
                    keyboard.modifiers = modifiers
                    keyboard.delegate?.keyboard(keyboard, releaseKey: key, modifiers: modifiers)
                }
            }
        }

    }

    deinit {
        glfwSetKeyCallback(window, nil)
    }

    fileprivate func key(_ key: Int32) -> KeyboardKey {
        switch key {
        case GLFW_KEY_UNKNOWN: return .unknown
        case GLFW_KEY_SPACE: return .space
        case GLFW_KEY_APOSTROPHE: return .apostrophe
        case GLFW_KEY_COMMA: return .comma
        case GLFW_KEY_MINUS: return .minus
        case GLFW_KEY_PERIOD: return .period
        case GLFW_KEY_SLASH: return .slash
        case GLFW_KEY_SEMICOLON: return .semicolon
        case GLFW_KEY_EQUAL: return .equal
        case GLFW_KEY_0: return .number0
        case GLFW_KEY_1: return .number1
        case GLFW_KEY_2: return .number2
        case GLFW_KEY_3: return .number3
        case GLFW_KEY_4: return .number4
        case GLFW_KEY_5: return .number5
        case GLFW_KEY_6: return .number6
        case GLFW_KEY_7: return .number7
        case GLFW_KEY_8: return .number8
        case GLFW_KEY_9: return .number9
        case GLFW_KEY_A: return .a
        case GLFW_KEY_B: return .b
        case GLFW_KEY_C: return .c
        case GLFW_KEY_D: return .d
        case GLFW_KEY_E: return .e
        case GLFW_KEY_F: return .f
        case GLFW_KEY_G: return .g
        case GLFW_KEY_H: return .h
        case GLFW_KEY_I: return .i
        case GLFW_KEY_J: return .j
        case GLFW_KEY_K: return .k
        case GLFW_KEY_L: return .l
        case GLFW_KEY_M: return .m
        case GLFW_KEY_N: return .n
        case GLFW_KEY_O: return .o
        case GLFW_KEY_P: return .p
        case GLFW_KEY_Q: return .q
        case GLFW_KEY_R: return .r
        case GLFW_KEY_S: return .s
        case GLFW_KEY_T: return .t
        case GLFW_KEY_U: return .u
        case GLFW_KEY_V: return .v
        case GLFW_KEY_W: return .w
        case GLFW_KEY_X: return .x
        case GLFW_KEY_Y: return .y
        case GLFW_KEY_Z: return .z
        case GLFW_KEY_LEFT_BRACKET: return .leftBracket
        case GLFW_KEY_RIGHT_BRACKET: return .rightBracket
        case GLFW_KEY_BACKSLASH: return .backslash
        case GLFW_KEY_GRAVE_ACCENT: return .graveAccent
        case GLFW_KEY_WORLD_1: return .world1
        case GLFW_KEY_WORLD_2: return .world2
        case GLFW_KEY_ESCAPE: return .escape
        case GLFW_KEY_ENTER: return .enter
        case GLFW_KEY_TAB: return .tab
        case GLFW_KEY_BACKSPACE: return .backspace
        case GLFW_KEY_INSERT: return .insert
        case GLFW_KEY_DELETE: return .delete
        case GLFW_KEY_RIGHT: return .right
        case GLFW_KEY_LEFT: return .left
        case GLFW_KEY_DOWN: return .down
        case GLFW_KEY_UP: return .up
        case GLFW_KEY_PAGE_UP: return .pageUp
        case GLFW_KEY_PAGE_DOWN: return .pageDown
        case GLFW_KEY_HOME: return .home
        case GLFW_KEY_END: return .end
        case GLFW_KEY_CAPS_LOCK: return .capsLock
        case GLFW_KEY_SCROLL_LOCK: return .scrollLock
        case GLFW_KEY_NUM_LOCK: return .numLock
        case GLFW_KEY_PRINT_SCREEN: return .printScreen
        case GLFW_KEY_PAUSE: return .pause
        case GLFW_KEY_F1: return .f1
        case GLFW_KEY_F2: return .f2
        case GLFW_KEY_F3: return .f3
        case GLFW_KEY_F4: return .f4
        case GLFW_KEY_F5: return .f5
        case GLFW_KEY_F6: return .f6
        case GLFW_KEY_F7: return .f7
        case GLFW_KEY_F8: return .f8
        case GLFW_KEY_F9: return .f9
        case GLFW_KEY_F10: return .f10
        case GLFW_KEY_F11: return .f11
        case GLFW_KEY_F12: return .f12
        case GLFW_KEY_F13: return .f13
        case GLFW_KEY_F14: return .f14
        case GLFW_KEY_F15: return .f15
        case GLFW_KEY_F16: return .f16
        case GLFW_KEY_F17: return .f17
        case GLFW_KEY_F18: return .f18
        case GLFW_KEY_F19: return .f19
        case GLFW_KEY_F20: return .f20
        case GLFW_KEY_F21: return .f21
        case GLFW_KEY_F22: return .f22
        case GLFW_KEY_F23: return .f23
        case GLFW_KEY_F24: return .f24
        case GLFW_KEY_F25: return .f25
        case GLFW_KEY_KP_0: return .keypad0
        case GLFW_KEY_KP_1: return .keypad1
        case GLFW_KEY_KP_2: return .keypad2
        case GLFW_KEY_KP_3: return .keypad3
        case GLFW_KEY_KP_4: return .keypad4
        case GLFW_KEY_KP_5: return .keypad5
        case GLFW_KEY_KP_6: return .keypad6
        case GLFW_KEY_KP_7: return .keypad7
        case GLFW_KEY_KP_8: return .keypad8
        case GLFW_KEY_KP_9: return .keypad9
        case GLFW_KEY_KP_DECIMAL: return .keypadDecimal
        case GLFW_KEY_KP_DIVIDE: return .keypadDivide
        case GLFW_KEY_KP_MULTIPLY: return .keypadMultiply
        case GLFW_KEY_KP_SUBTRACT: return .keypadSubtract
        case GLFW_KEY_KP_ADD: return .keypadAdd
        case GLFW_KEY_KP_ENTER: return .keypadEnter
        case GLFW_KEY_KP_EQUAL: return .keypadEqual
        case GLFW_KEY_LEFT_SHIFT: return .leftShift
        case GLFW_KEY_LEFT_CONTROL: return .leftControl
        case GLFW_KEY_LEFT_ALT: return .leftAlt
        case GLFW_KEY_LEFT_SUPER: return .leftSuper
        case GLFW_KEY_RIGHT_SHIFT: return .rightShift
        case GLFW_KEY_RIGHT_CONTROL: return .rightControl
        case GLFW_KEY_RIGHT_ALT: return .rightAlt
        case GLFW_KEY_RIGHT_SUPER: return .rightSuper
        case GLFW_KEY_MENU: return .menu
        default: return .unknown
        }
    }

    fileprivate func modifiers(_ modes: Int32) -> KeyboardModifier {
        var modifiers: [KeyboardModifier] = []
        if modes & GLFW_MOD_SHIFT != 0 {
            modifiers.append(.shift)
        }
        if modes & GLFW_MOD_CONTROL != 0 {
            modifiers.append(.control)
        }
        if modes & GLFW_MOD_ALT != 0 {
            modifiers.append(.alt)
        }
        if modes & GLFW_MOD_SUPER != 0 {
            modifiers.append(.super)
        }
        return KeyboardModifier(modifiers)
    }

}
