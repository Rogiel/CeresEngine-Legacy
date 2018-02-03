//
// Created by Rogiel Sulzbach on 10/21/17.
//

import GameEngine

public class Star: BaseEntity, CelestialBody,
        Positioning, Rotating, Updatable, Renderable {

    // MARK: - Star parameter & attributes

    /// The planet name
    public private(set) var name: String

    /// The planet mass
    public var mass: Double

    /// The planet radius
    public var radius: Double

    // MARK: - Star relationships

    /// The planet's parent object
    public var parent: Transformable? {
        return nil
    }

    // MARK: - Rendering

    /// The planet's 3D model
    public var model: Model

    /// The planet's position relative to its parent, if any.
    public var position: Vector3<Double> = Vector3<Double>()

    /// The planet's world position
    public var worldPosition: Vector3<Double> { return position }

    /// The planet rotation
    public var rotation: Vector3<Double> {
        return Vector3<Double>(0.0)
    }

    /// Whether the planet should cast shadows
    public var castShadows: Bool {
        return true
    }

    /// The planet transformation matrix
    public var transform: Matrix4x4<Double> {
        return SGLMath.rotate(SGLMath.rotate(SGLMath.rotate(
                SGLMath.scale(
                        SGLMath.translate(Matrix4x4<Double>(), position),
                        Vector3<Double>(radius * 2.0)),
                rotation.x, Vector3<Double>(1.0, 0.0, 0.0)),
                rotation.y, Vector3<Double>(0.0, 1.0, 0.0)),
                rotation.z, Vector3<Double>(0.0, 0.0, 1.0))
    }

    public init(name: String, model: Model, mass: Double, radius: Double) {
        self.name = name
        self.model = model
        self.mass = mass
        self.radius = radius
    }

    public func update(delta: Double) {

    }


}
