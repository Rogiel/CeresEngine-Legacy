//
// Created by Rogiel Sulzbach on 10/20/17.
//

import Foundation
import GameEngine

public class Moon : BaseEntity, CelestialBody,
        Positioning, Rotating, Updatable, Renderable {
    // MARK: - Planet parameter & attributes

    /// The planet name
    public private(set) var name: String

    /// The planet mass
    public var mass: Double

    /// The planet radius
    public var radius: Double

    /// The planet rotation period
    public var rotationPeriod: Double

    /// The planet orbit around its primary
    public var orbit: Orbit!

    // MARK: - Planet relationships

    /// The star this planet is orbiting around
    public var planet: Planet

    /// The planet's parent object
    public var parent: Transformable? {
        return planet
    }

    /// A list of all crafts that are orbiting this planet
    public var crafts: [Craft] = []

    // MARK: - Rendering

    /// The planet's 3D model
    public var model: Model

    /// The planet's position relative to its parent, if any.
    public private(set) var position: Vector3<Double> = Vector3<Double>()

    /// The planet's world position
    public var worldPosition: Vector3<Double> {
        return planet.position + position
    }

    /// The planet rotation
    public var rotation: Vector3<Double> = Vector3<Double>()

    /// Whether the planet should cast shadows
    public var castShadows: Bool {
        return true
    }

    /// The planet transformation matrix
    public var transform: Matrix4x4<Double> {
        return SGLMath.rotate(SGLMath.rotate(SGLMath.rotate(
                SGLMath.scale(
                        SGLMath.translate(Matrix4x4<Double>(), worldPosition),
                        Vector3<Double>(radius * 2.0)),
                rotation.x, Vector3<Double>(1.0, 0.0, 0.0)),
                rotation.y, Vector3<Double>(0.0, 1.0, 0.0)),
                rotation.z, Vector3<Double>(0.0, 0.0, 1.0))
    }

    public init(name: String, planet: Planet, model: Model, orbit: Orbit, radius: Double, rotationPeriod: Double) {
        self.name = name
        self.planet = planet
        self.model = model
        self.rotationPeriod = rotationPeriod
        self.orbit = orbit
        self.mass = orbit.secondaryMass
        self.radius = radius
    }

    private var time: Double = Date().timeIntervalSince1970 - 946684800

    public func update(delta: Double) {
        rotation.y += (1 / rotationPeriod) * Double.pi * 2 * delta

        if let orbit = orbit?[at: time] {
            let radius = orbit.radius
            position = Vector3(radius.x, radius.z, radius.y)
        }

        time += delta
    }



}
