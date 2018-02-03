//
//  Camera.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import Foundation
import SGLMath

public protocol Camera : class {

    /// The camera position
    var position: Vector3<Double> { get set }

    /// The camera apparent position
    var viewPosition: Vector3<Double> { get }

    /// The camera front pointing vector
    var front: Vector3<Double> { get }

    /// The camera back pointing vector
    var back: Vector3<Double> { get }

    /// The camera up pointing vector
    var up: Vector3<Double> { get }

    /// The camera down pointing vector
    var down: Vector3<Double> { get }

    /// The camera right pointing vector
    var right: Vector3<Double> { get }

    /// The camera left pointing vector
    var left: Vector3<Double> { get }

    /// The camera yaw
    var yaw: Double { get set }

    /// The camera pitch
    var pitch: Double { get set }

    /// The camera zoom level
    var zoom: Double { get set }

    /// The camera field of view
    var fieldOfView: Double { get set }

    /// The distance where objects near the camera start clipping
    var nearPlane: Double { get set }

    /// The distance where objects far from the camera start clipping
    var farPlane: Double { get set }

    /// The screen aspect ration
    var aspectRatio: Double { get set }

    /// The camera projection matrix
    var projection: Matrix4x4<Double> { get }

    /// The camera view matrix
    var view: Matrix4x4<Double> { get }

}

public extension Camera {

    public func lookAt(point: Vector3<Double>) {
        let direction = point - position

        let pitch = asin(direction.y / length(direction));
        let yaw: Double;

        if abs(direction.z) < 0.00001 {
            // Special case
            if direction.x > 0 {
                yaw = Double.pi / 2.0;
            } else if direction.x < 0 {
                yaw = -Double.pi / 2.0;
            } else {
                yaw = 0.0;
            }
        } else {
            yaw = atan2(direction.x, direction.z);
        }

        self.pitch = pitch
        self.yaw = yaw
    }

}

public class FirstPersonCamera: Camera {

    /// The camera position
    public var position: Vector3<Double> = Vector3<Double>()

    /// The camera apparent position
    public var viewPosition: Vector3<Double> {
        return position - front * zoom
    }

    /// The camera front pointing vector
    public var front: Vector3<Double> {
        return normalize(Vector3<Double>(
                x: cos(radians(yaw)) * cos(radians(pitch)),
                y: sin(radians(pitch)),
                z: sin(radians(yaw)) * cos(radians(pitch))
        ))
    }

    /// The camera back pointing vector
    public var back: Vector3<Double> {
        return -front
    }

    /// The camera up pointing vector
    public var up: Vector3<Double> {
        return Vector3<Double>(0.0, 1.0, 0.0)
    }

    /// The camera down pointing vector
    public var down: Vector3<Double> {
        return -up
    }

    /// The camera right pointing vector
    public var right: Vector3<Double> {
        return normalize(front * up)
    }

    /// The camera left pointing vector
    public var left: Vector3<Double> {
        return -right
    }

    /// The camera yaw
    public var yaw: Double = -90.0

    /// The camera pitch
    public var pitch: Double = 0.0

    /// The camera zoom level
    public var zoom: Double = 0.0

    /// The camera field of view
    public var fieldOfView: Double = 45.0

    /// The distance where objects near from the camera start clipping
    public var nearPlane: Double = 0.1

    /// The distance where objects far from the camera start clipping
    public var farPlane: Double = 100.0

    /// The screen aspect ration
    public var aspectRatio: Double = Double(800) / Double(600)

    /// The camera projection matrix
    public var projection: Matrix4x4<Double> {
        return SGLMath.infinitePerspective(radians(fieldOfView), aspectRatio, nearPlane)
    }

    /// The camera view matrix
    public var view: Matrix4x4<Double> {
        return SGLMath.lookAt(position - front * zoom, position + front, up)
    }

    /// Creates a new first person camera object
    public init(position: Vector3<Double> = Vector3<Double>(), yaw: Double = 0.0, pitch: Double = 0.0) {
        self.position = position
        self.yaw = yaw
        self.pitch = pitch
    }

}

public class OrbitalCamera: Camera {

    public var object: Positioning

    /// The camera position
    public var position: Vector3<Double> {
        get {
            return object.worldPosition
        }
        set {
        }
    }

    /// The camera apparent position
    public var viewPosition: Vector3<Double> {
        return position - front * zoom
    }

    /// The camera front pointing vector
    public var front: Vector3<Double> {
        return normalize(Vector3<Double>(
                x: cos(radians(yaw)) * cos(radians(pitch)),
                y: sin(radians(pitch)),
                z: sin(radians(yaw)) * cos(radians(pitch))
        ))
    }

    /// The camera back pointing vector
    public var back: Vector3<Double> {
        return -front
    }

    /// The camera up pointing vector
    public var up: Vector3<Double> {
        return Vector3<Double>(0.0, 1.0, 0.0)
    }

    /// The camera down pointing vector
    public var down: Vector3<Double> {
        return -up
    }

    /// The camera right pointing vector
    public var right: Vector3<Double> {
        return normalize(front * up)
    }

    /// The camera left pointing vector
    public var left: Vector3<Double> {
        return -right
    }

    /// The camera yaw
    public var yaw: Double = -90.0

    /// The camera pitch
    public var pitch: Double = 0.0

    /// The camera zoom level
    public var zoom: Double = 0.0

    /// The camera field of view
    public var fieldOfView: Double = 45.0

    /// The distance where objects near from the camera start clipping
    public var nearPlane: Double = 0.0001

    /// The distance where objects far from the camera start clipping
    public var farPlane: Double = 100.0

    /// The screen aspect ration
    public var aspectRatio: Double = Double(800) / Double(600)

    /// The camera projection matrix
    public var projection: Matrix4x4<Double> {
        return SGLMath.infinitePerspective(radians(fieldOfView), aspectRatio, nearPlane)
//        return SGLMath.perspective(radians(fieldOfView), aspectRatio, nearPlane, farPlane)
    }

    /// The camera view matrix
    public var view: Matrix4x4<Double> {
        return SGLMath.lookAt(position - front * zoom, position + front, up)
    }

    /// Creates a new first person camera object
    public init(object: Positioning, yaw: Double = 0.0, pitch: Double = 0.0) {
        self.object = object
        self.yaw = yaw
        self.pitch = pitch
    }

}