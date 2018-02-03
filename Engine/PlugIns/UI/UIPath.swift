//
// Created by Rogiel Sulzbach on 10/28/17.
//

import Foundation
import CNanoVG

public class UIPath {

    private var context: DrawContext

    fileprivate init(context: DrawContext) {
        self.context = context
    }

    /// Starts new sub-path with specified point as first point.
    @discardableResult
    public func move(to point: UIPoint) -> UIPath {
        return self
    }

    /// Adds line segment from the last point in the path to the specified point.
    @discardableResult
    public func line(to point: UIPoint) -> UIPath {
        return self
    }

    /// Adds cubic bezier segment from last point in the path via two control points to the specified point.
    @discardableResult
    public func bezier(to point: UIPoint) -> UIPath {
        return self
    }

    /// Adds quadratic bezier segment from last point in the path via a control point to the specified point.
    @discardableResult
    public func quad(to point: UIPoint/*TODO parameter */) -> UIPath {
        return self
    }

    /// Adds an arc segment at the corner defined by the last path point, and two specified points.
    @discardableResult
    public func arc(from: UIPoint, to: UIPoint,
                    radius: Double) -> UIPath {
        return self
    }

    // Closes current sub-path with a line segment.
    @discardableResult
    public func close() -> UIPath {
        return self
    }

    // Sets the current sub-path winding, see NVGwinding and NVGsolidity.
    @discardableResult
    public func winding(/*TODO parameter */) -> UIPath {
        return self
    }

    // Creates new circle arc shaped sub-path. The arc center is at cx,cy, the arc radius is r,
    // and the arc is drawn from angle a0 to a1, and swept in direction dir (NVG_CCW, or NVG_CW).
    // Angles are specified in radians.
    @discardableResult
    public func arc(center: UIPoint, radius: Double,
                    from: Double, to: Double,
                    direction: UIPathDrawDirection) -> UIPath {
        return self
    }

    // Creates new rectangle shaped sub-path.
    @discardableResult
    public func rect(origin: UIPoint, size: UIPoint) -> UIPath {
        return self.rect(origin: origin, size: size, borderRadius: nil)
    }

    // Creates new rounded rectangle shaped sub-path.
    @discardableResult
    public func rect(origin: UIPoint, size: UIPoint,
                     borderRadius: Double? = nil) -> UIPath {
        return self.rect(origin: origin, size: size,
                topLeftBorderRadius: borderRadius,
                topRightBorderRadius: borderRadius,
                bottomLeftBorderRadius: borderRadius,
                bottomRightBorderRadius: borderRadius)
    }

    // Creates new rounded rectangle shaped sub-path with varying radii for each corner.
    @discardableResult
    public func rect(origin: UIPoint, size: UIPoint,
                     topLeftBorderRadius: Double? = nil,
                     topRightBorderRadius: Double? = nil,
                     bottomLeftBorderRadius: Double? = nil,
                     bottomRightBorderRadius: Double? = nil) -> UIPath {
        nvgRoundedRectVarying(
                context,
                Float(origin.x), Float(origin.y),
                Float(size.x), Float(size.y),
                Float(topLeftBorderRadius ?? 0),
                Float(topRightBorderRadius ?? 0),
                Float(bottomRightBorderRadius ?? 0),
                Float(bottomRightBorderRadius ?? 0)
        )
        return self
    }

    // Creates new ellipse shaped sub-path.
    @discardableResult
    public func ellipse(center: UIPoint, radius: UIPoint) -> UIPath {
        return self
    }

    // Creates new circle shaped sub-path.
    @discardableResult
    public func circle(center: UIPoint, radius: Double) -> UIPath {
        return self
    }

    // Fills the current path with current fill style.
    @discardableResult
    public func fill(color: UIColor, antiAliasing: Bool = true) -> UIPath {
        nvgFillColor(
                context,
                nvgRGBA(
                        UInt8(color.red * 255),
                        UInt8(color.green * 255),
                        UInt8(color.blue * 255),
                        UInt8(color.alpha * 255)
                )
        )
        nvgFill(context)
        return self
    }

//    // Fills the current path with current fill style.
//    public func fill(paint: UIPaint, antiAliasing: Bool = true) -> UIPath {
//        return self
//    }

    // Fills the current path with current stroke style.
    @discardableResult
    public func stroke(color: UIColor, width: Double = 1.0,
                       miterLimit: Double = 0,
                       lineJoin: UIPathLineJoin = .miter,
                       lineCap: UIPathLineCap = .butt,
                       antiAliasing: Bool = true) -> UIPath {
        nvgStrokeColor(
                context,
                nvgRGBA(
                        UInt8(color.red * 255),
                        UInt8(color.green * 255),
                        UInt8(color.blue * 255),
                        UInt8(color.alpha * 255)
                )
        )
        nvgStrokeWidth(
                context,
                Float(width)
        )
        nvgStroke(context)

        return self
    }

//    // Fills the current path with current stroke style.
//    public func stroke(paint: UIPaint, width: Double = 1.0,
//                       miterLimit: Double = 0,
//                       lineJoin: UIPathLineJoin = .miter,
//                       lineCap: UIPathLineCap = .butt,
//                       antiAliasing: Bool = true) -> UIPath {
//        return self
//    }

    public static func draw(_ context: DrawContext, _ body: (UIPath)->Void) {
        let path = UIPath(context: context)

        nvgBeginPath(context)
        body(path)
    }

}

public enum UIPathDrawDirection {
    case clockWise
    case counterClockWise
}

public enum UIPathLineCap {
    case butt
    case round
    case square
}

public enum UIPathLineJoin {
    case miter
    case round
    case bevel
}