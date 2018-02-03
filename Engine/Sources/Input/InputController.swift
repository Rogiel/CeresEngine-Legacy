//
// Created by Rogiel Sulzbach on 10/23/17.
//

open class InputController: MouseDelegate, KeyboardDelegate {

    public init() {

    }

    // MARK: - Mouse

    /// A list of currently controlled mouses
    public var mouses: [Mouse] = [] {
        willSet {
            for mouse in mouses {
                if !newValue.contains(where: { $0 === mouse }) {
                    mouse.delegate = nil
                }
            }
        }

        didSet {
            for mouse in mouses {
                mouse.delegate = self
            }
        }
    }

    /// A delegate method called whenever the mouse location is updated
    open func mouse(_ mouse: Mouse, didMoveToLocation location: Mouse.Position) {

    }

    open func mouse(_ mouse: Mouse, pressedButton button: MouseButton) {
    }

    open func mouse(_ mouse: Mouse, releasedButton button: MouseButton) {

    }

    open func mouse(_ mouse: Mouse, scrollBy scrollOffset: Double, direction: MouseScrollDirection) {
    }

    open func mouseShouldHideCursor(_ mouse: Mouse) -> Bool {
        return true
    }

    open func mouseWillHideCursor(_ mouse: Mouse) {
    }

    open func mouseDidHideCursor(_ mouse: Mouse) {
    }

    open func mouse(_ mouse: Mouse, shouldChangeCursorShape shape: MouseCursorShape) -> Bool {
        return true
    }

    open func mouse(_ mouse: Mouse, willChangeCursorShape shape: MouseCursorShape) {
    }

    open func mouse(_ mouse: Mouse, didChangeCursorShape shape: MouseCursorShape) {
    }

    // MARK: - Keyboard

    /// A list of currently controlled keyboards
    public var keyboards: [Keyboard] = [] {
        willSet {
            for keyboard in keyboards {
                if !newValue.contains(where: { $0 === keyboard }) {
                    keyboard.delegate = nil
                }
            }
        }

        didSet {
            for keyboard in keyboards {
                keyboard.delegate = self
            }
        }
    }

    /// A delegate method that gets called when a key is pressed on the keyboard
    open func keyboard(_ keyboard: Keyboard, pressKey: KeyboardKey, modifiers: KeyboardModifier, repeat: Bool) {

    }

    /// A delegate method that gets called when a key is released on the keyboard
    open func keyboard(_ keyboard: Keyboard, releaseKey: KeyboardKey, modifiers: KeyboardModifier) {

    }


}
