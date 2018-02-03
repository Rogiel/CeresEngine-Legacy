//
// Created by Rogiel Sulzbach on 10/13/17.
//

import Foundation
import GameEngine
import SGLMath

public class Planet: BaseEntity, CelestialBody,
        Positioning, Rotating, Updatable, Renderable {

    // MARK: - Planet parameter & attributes

    /// The planet name
    public private(set) var name: String

    /// The planet type
    public var type: PlanetType = .default

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
    public var star: Star

    /// The planet's parent object
    public var parent: Transformable? {
        return star
    }

//    /// A list of all moons that orbit this planet
//    public var moons: [Moon] = []

    /// A list of all crafts that are orbiting this planet
    public var crafts: [Craft] = []

    // MARK: - Rendering

    /// The planet's 3D model
    public var model: Model

    /// The planet's position relative to its parent, if any.
    public private(set) var position: Vector3<Double> = Vector3<Double>()

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

    public init(name: String, star: Star, model: Model, orbit: Orbit, radius: Double, rotationPeriod: Double) {
        self.name = name
        self.star = star
        self.model = model
        self.rotationPeriod = rotationPeriod
        self.orbit = orbit
        self.mass = orbit.secondaryMass
        self.radius = radius
    }

//    private var time: Double = Date().timeIntervalSince1970 - 946684800
    private var time: Double = 1517875200 - 946684800

    public func update(delta: Double) {
        rotation.y += (1 / rotationPeriod) * Double.pi * 2 * delta

        let currentOrbit = orbit?[at: time]
        if let orbit = currentOrbit {
            let radius = orbit.radius
            position = Vector3(radius.x, radius.z, radius.y)
        }

        time += delta
    }

    public var date: Date {
        return Date(timeInterval: time, since: Date(timeIntervalSince1970: 946684800))
    }


}

public extension Planet {

    public struct SerializationData: Codable {
        public var id: String
        public var name: String

        public var mass: Double
        public var radius: Double

        public struct TextureSet: Codable {
            public var diffuse: String
            public var specular: String
            public var normal: String
        }

        public var textures: TextureSet

        public struct OrbitalParameters: Codable {
            public var primaryID: String?
            public var semiMajorAxis: Double
            public var eccentricity: Double
            public var inclination: Double
            public var ascendingNode: Double
            public var argumentOfPeriapsis: Double
        }

        public var orbit: OrbitalParameters!

    }

//    public convenience init(data: SerializationData) {
//        self.init(name: data.name, model: SimpleModel(mesh: SimpleMesh(vertices: [])))
//        self.mass = data.mass
//        self.radius = data.radius
//
//        self.orbit = KeplerOrbit(
//                primaryMass: 0,
//                secondaryMass: data.mass,
//                semiMajorAxis: data.orbit.semiMajorAxis,
//                eccentricity: data.orbit.eccentricity,
//                inclination: data.orbit.inclination,
//                ascendingNode: data.orbit.ascendingNode,
//                argumentOfPeriapsis: data.orbit.argumentOfPeriapsis,
//                trueAnomaly: 0)
//    }

    public var serializationData: SerializationData {
        return .init(
                id: "2",
                name: name,
                mass: mass,
                radius: radius,
                textures: .init(
                        diffuse: "",
                        specular: "",
                        normal: ""
                ),
                orbit: orbit != nil ? SerializationData.OrbitalParameters.init(
                        primaryID: "",
                        semiMajorAxis: orbit.semiMajorAxis,
                        eccentricity: orbit.eccentricity,
                        inclination: orbit.inclination,
                        ascendingNode: orbit.longitudeOfAscedingNode,
                        argumentOfPeriapsis: orbit.argumentOfPeriapsis
                ) : nil
        )
    }

}