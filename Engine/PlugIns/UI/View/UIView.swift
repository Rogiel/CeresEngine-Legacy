//
// Created by Rogiel Sulzbach on 10/28/17.
//

import Foundation

open class UIView {

    public var frame: UIRect

    public private(set) var superview: UIView!
    public private(set) var subviews: [UIView] = []

    public init() {
        self.frame = UIRect()
    }

    public init(frame: UIRect) {
        self.frame = frame
    }

    open func draw(context: DrawContext) {

    }

    public func add(subview view: UIView) {
        if subviews.contains(where: { $0 === view }) {
            return
        }
        view.superview = self
        subviews.append(view)
    }

    public func remove(subview view: UIView) {
        if !subviews.contains(where: { $0 === view }) {
            return
        }
        view.superview = nil
        subviews = subviews.filter { $0 !== view }
    }

}
