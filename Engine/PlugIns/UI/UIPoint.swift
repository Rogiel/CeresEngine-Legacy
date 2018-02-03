//
// Created by Rogiel Sulzbach on 10/28/17.
//

import Foundation

public struct UIPoint {

    public var x: Double
    public var y: Double

    public init(x: Double, y: Double) {
        self.x = x
        self.y = y
    }

    public init() {
        self.init(x: 0, y: 0)
    }

}

public struct UIRect {

    public var origin: UIPoint
    public var size:   UIPoint

    public init(origin: UIPoint, size: UIPoint) {
        self.origin = origin
        self.size = size
    }

    public init() {
        self.init(origin: UIPoint(), size: UIPoint())
    }

    public var width: Double {
        get { return size.x }
        set { size.x = newValue }
    }

    public var height: Double {
        get { return size.y }
        set { size.y = newValue }
    }

    public var w: Double {
        get { return size.x }
        set { size.x = newValue }
    }

    public var h: Double {
        get { return size.y }
        set { size.y = newValue }
    }

    public var x: Double {
        get { return origin.x }
        set { origin.x = newValue }
    }

    public var y: Double {
        get { return origin.y }
        set { origin.y = newValue }
    }

}