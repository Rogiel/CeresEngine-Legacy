//
// Created by Rogiel Sulzbach on 10/22/17.
//

public protocol Mouse : class {

    /// A type alias to a mouse position vector
    /// These positions are normalized in relation to the window.
    typealias Position = Vector2<Double>

    /// The mouse delegate
    /// This delegate will be called with updates on mouse movement,
    /// scrolling and clicking events
    var delegate: MouseDelegate? { get set }

    /// The mouse cursor shape
    var cursorShape: MouseCursorShape { get set }

    /// The currently pressed moused buttons
    var buttons: [MouseButton] { get }

    /// The current mouse position
    var position: Position { get }

    /// A flag indicating if the cursor is visible or not
    var visible: Bool { get set }

}

public extension Mouse {

    /// Returns `true` if the `left` mouse button is pressed
    public var isLeftButtonPressed: Bool {
        return self.buttons.contains(.left)
    }

    /// Returns `true` if the `middle` mouse button is pressed
    public var isMiddleButtonPressed: Bool {
        return self.buttons.contains(.middle)
    }

    /// Returns `true` if the `right` mouse button is pressed
    public var isRightButtonPressed: Bool {
        return self.buttons.contains(.right)
    }

}

public protocol MouseDelegate : class {
    /// A delegate method called whenever the mouse location is updated
    func mouse(_ mouse: Mouse, didMoveToLocation location: Mouse.Position)

    func mouse(_ mouse: Mouse,  pressedButton: MouseButton)
    func mouse(_ mouse: Mouse, releasedButton: MouseButton)

    func mouse(_ mouse: Mouse, scrollBy scrollOffset: Double, direction: MouseScrollDirection)

    func mouseShouldHideCursor(_ mouse: Mouse) -> Bool
    func mouseWillHideCursor(_ mouse: Mouse)
    func mouseDidHideCursor (_ mouse: Mouse)

    func mouse(_ mouse: Mouse, shouldChangeCursorShape shape: MouseCursorShape) -> Bool
    func mouse(_ mouse: Mouse, willChangeCursorShape   shape: MouseCursorShape)
    func mouse(_ mouse: Mouse, didChangeCursorShape    shape: MouseCursorShape)
}

public enum MouseButton {
    case left
    case middle
    case right

    case button1, button2, button3, button4,
         button5, button6, button7, button8

    public var isLeft: Bool {
        return self == .left
    }
    public var isMiddle: Bool {
        return self == .middle
    }
    public var isRight: Bool {
        return self == .right
    }
}

public enum MouseScrollDirection {
    case vertical
    case horizontal

    public var isVertical: Bool {
        return self == .vertical
    }
    public var isHorizontal: Bool {
        return self == .horizontal
    }
}

public enum MouseCursorShape {
    case arrow
    case ibeam
    case crosshair
    case hand
    case horizontalResize
    case verticalResize

    public var isArrow: Bool {
        return self == .arrow
    }
    public var isIbeam: Bool {
        return self == .ibeam
    }
    public var isCrosshair: Bool {
        return self == .crosshair
    }
    public var isHand: Bool {
        return self == .hand
    }

    public var isHorizontalResize: Bool {
        return self == .horizontalResize
    }
    public var isVerticalResize: Bool {
        return self == .verticalResize
    }

}