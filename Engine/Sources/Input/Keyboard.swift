//
// Created by Rogiel Sulzbach on 10/22/17.
//

public protocol Keyboard : class {

    /// The keyboard delegate
    var delegate: KeyboardDelegate? { get set }

    /// The currently pressed keyboard keys
    var keys: [KeyboardKey] { get }

    /// The current keyboard modifier keys
    var modifiers: KeyboardModifier { get }

}

public extension Keyboard {

    /// Check if the given key is currently pressed with a optional
    /// modifier key set
    public func pressed(key: KeyboardKey, modifiers: KeyboardModifier = []) -> Bool {
        return pressed(keys: [key], modifiers: modifiers)
    }

    /// Check if the given keys are currently pressed with a optional
    /// modifier key set
    public func pressed(keys: [KeyboardKey], modifiers: KeyboardModifier = []) -> Bool {
        for key in keys {
            if !self.keys.contains(key) {
                return false
            }
        }

        // if the modifiers value is empty, the key combination is pressed!
        if modifiers.isEmpty {
            return true
        }
        return self.modifiers.contains(modifiers)
    }

}

public protocol KeyboardDelegate : class {

    /// A delegate method that gets called when a key is pressed on the keyboard
    func keyboard(_ keyboard: Keyboard,   pressKey: KeyboardKey, modifiers: KeyboardModifier, repeat: Bool)

    /// A delegate method that gets called when a key is released on the keyboard
    func keyboard(_ keyboard: Keyboard, releaseKey: KeyboardKey, modifiers: KeyboardModifier)

}

public enum KeyboardKey {
    case unknown
    case space
    case apostrophe   /* '  */
    case comma   /* , */
    case minus   /* - */
    case period   /* . */
    case slash   /* / */
    case semicolon   /* ; */
    case equal   /* = */

    case number0, number1, number2, number3, number4, number5, number6, number7, number8, number9
    case a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z

    case leftBracket   /* [ */
    case rightBracket   /* ] */
    case backslash   /* \ */
    case graveAccent   /* ` */

    case world1   /* non-us #1 */
    case world2   /* non-us #2 */

    case escape
    case enter
    case tab
    case backspace
    case insert
    case delete
    case right
    case left
    case down
    case up

    case pageUp
    case pageDown
    case home
    case end
    case capsLock
    case scrollLock
    case numLock
    case printScreen
    case pause

    case f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
         f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25

    case keypad0, keypad1, keypad2, keypad3, keypad4,
         keypad5, keypad6, keypad7, keypad8, keypad9

    case keypadDecimal
    case keypadDivide
    case keypadMultiply
    case keypadSubtract
    case keypadAdd
    case keypadEnter
    case keypadEqual

    case leftShift
    case leftControl
    case leftAlt
    case leftSuper
    case rightShift
    case rightControl
    case rightAlt
    case rightSuper
    case menu

    public var isNumberKey: Bool {
        switch self {
        case .number0: fallthrough
        case .keypad0: fallthrough
        case .number1: fallthrough
        case .keypad1: fallthrough
        case .number2: fallthrough
        case .keypad2: fallthrough
        case .number3: fallthrough
        case .keypad3: fallthrough
        case .number4: fallthrough
        case .keypad4: fallthrough
        case .number5: fallthrough
        case .keypad5: fallthrough
        case .number6: fallthrough
        case .keypad6: fallthrough
        case .number7: fallthrough
        case .keypad7: fallthrough
        case .number8: fallthrough
        case .keypad8: fallthrough
        case .number9: fallthrough
        case .keypad9: return true
        default: return false
        }
    }

    public var numericValue: Int? {
        switch self {
        case .number0: fallthrough
        case .keypad0: return 0

        case .number1: fallthrough
        case .keypad1: return 0

        case .number2: fallthrough
        case .keypad2: return 0

        case .number3: fallthrough
        case .keypad3: return 0

        case .number4: fallthrough
        case .keypad4: return 0

        case .number5: fallthrough
        case .keypad5: return 0

        case .number6: fallthrough
        case .keypad6: return 0

        case .number7: fallthrough
        case .keypad7: return 0

        case .number8: fallthrough
        case .keypad8: return 0

        case .number9: fallthrough
        case .keypad9: return 0

        default: return nil
        }
    }

    public var isCharacter: Bool {
        switch self {
        case .a: return true
        case .b: return true
        case .c: return true
        case .d: return true
        case .e: return true
        case .f: return true
        case .g: return true
        case .h: return true
        case .i: return true
        case .j: return true
        case .k: return true
        case .l: return true
        case .m: return true
        case .n: return true
        case .o: return true
        case .p: return true
        case .q: return true
        case .r: return true
        case .s: return true
        case .t: return true
        case .u: return true
        case .v: return true
        case .w: return true
        case .x: return true
        case .y: return true
        case .z: return true
        case .number9: return true
        case .number8: return true
        case .number7: return true
        case .number6: return true
        case .number5: return true
        case .number4: return true
        case .number3: return true
        case .number2: return true
        case .number1: return true
        case .number0: return true
        default: return false
        }
    }

    public var character: Character? {
        switch self {
        case .a: return Character("a")
        case .b: return Character("b")
        case .c: return Character("c")
        case .d: return Character("d")
        case .e: return Character("e")
        case .f: return Character("f")
        case .g: return Character("g")
        case .h: return Character("h")
        case .i: return Character("i")
        case .j: return Character("j")
        case .k: return Character("k")
        case .l: return Character("l")
        case .m: return Character("m")
        case .n: return Character("n")
        case .o: return Character("o")
        case .p: return Character("p")
        case .q: return Character("q")
        case .r: return Character("r")
        case .s: return Character("s")
        case .t: return Character("t")
        case .u: return Character("u")
        case .v: return Character("v")
        case .w: return Character("w")
        case .x: return Character("x")
        case .y: return Character("y")
        case .z: return Character("z")
        case .number9: return Character("9")
        case .number8: return Character("8")
        case .number7: return Character("7")
        case .number6: return Character("6")
        case .number5: return Character("5")
        case .number4: return Character("4")
        case .number3: return Character("3")
        case .number2: return Character("2")
        case .number1: return Character("1")
        case .number0: return Character("0")
        default: return nil
        }
    }
}

public struct KeyboardModifier: OptionSet {
    public let rawValue: Int

    public init(rawValue: Int) {
        self.rawValue = rawValue
    }

    public static let shift = KeyboardModifier(rawValue: 1 << 0)
    public static let control = KeyboardModifier(rawValue: 1 << 1)
    public static let alt = KeyboardModifier(rawValue: 1 << 3)
    public static let `super` = KeyboardModifier(rawValue: 1 << 4)

    public var hasShift: Bool {
        return self.contains(KeyboardModifier.shift)
    }
    public var hasControl: Bool {
        return self.contains(KeyboardModifier.control)
    }
    public var hasAlt: Bool {
        return self.contains(KeyboardModifier.alt)
    }
    public var hasSuper: Bool {
        return self.contains(KeyboardModifier.`super`)
    }
}
