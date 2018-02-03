//
// Created by Rogiel Sulzbach on 10/14/17.
//

import Foundation
import SGLMath

/// The gravitational constant. Used when computing orbital parameters.
fileprivate let G = 6.67384e-11;

/// A protocol that represents a object orbit. Each orbit parameter can either be
/// calculated on the fly or rely on precomputed parameters to speed-up orbit
/// calculations.
public protocol Orbit {

    // MARK: - System parameters

    /// The orbit primary body mass
    var primaryMass: Double { get }

    /// The orbit secondary body mass
    var secondaryMass: Double { get }

    // MARK: - Cartesian parameters

    /// The orbit radius relative to the center of the primary body
    var radius: Vector3<Double> { get }

    /// The orbit radius relative to the center of the primary body.
    /// This parameter represents the scalar value of the radius.
    var scalarRadius: Double { get }

    /// The orbit velocity relative to the center of the primary body
    var velocity: Vector3<Double> { get }

    /// The orbit velocity relative to the center of the primary body.
    /// This parameter represents the scalar value of the velocity.
    var scalarVelocity: Double { get }

    // MARK: - Keplerian parameters

    /// The orbit semi-major axis
    ///
    /// The sum of the periapsis and apoapsis distances divided by
    /// two. For circular orbits, the semimajor axis is the distance
    /// between the centers of the bodies, not the distance of the bodies
    /// from the center of mass. For a parabolic orbit, this is infinite.
    /// For a hyperbolic orbit, if you graph the two-branched hyperbola on
    /// the Cartesian plane, is half the distance between periapsis and
    /// the opposing vertex and is usually treated as negative for orbital
    /// energy purposes.
    var semiMajorAxis: Double { get }

    /// The orbit eccentricity
    ///
    /// The shape of the ellipse, describing how much it is elongated
    /// compared to a circle
    var eccentricity: Double { get }

    /// The orbit inclination
    ///
    /// The vertical tilt of the ellipse with respect to the reference
    /// plane, measured at the ascending node (where the orbit passes
    /// upward through the reference plane). Tilt angle is measured
    /// perpendicular to line of intersection between orbital plane
    /// and reference plane. Any three points on an ellipse will define
    /// the ellipse orbital plane. The plane and the ellipse are both
    /// two-dimensional objects defined in three-dimensional space.
    var inclination: Double { get }

    /// The orbit longitude of the ascending node
    ///
    /// Horizontally orients the ascending node of the ellipse (where
    /// the orbit passes upward through the reference plane) with respect
    /// to the reference frame's vernal point.
    var longitudeOfAscedingNode: Double { get }

    /// The orbit argument of periapsis
    ///
    /// Defines the orientation of the ellipse in the orbital plane,
    /// as an angle measured from the ascending node to the periapsis
    /// (the closest point the satellite object comes to the primary
    /// object around which it orbits).
    var argumentOfPeriapsis: Double { get }

    /// The orbit true anomaly
    ///
    /// Defines the position of the orbiting body along the ellipse at a
    /// specific time (the "epoch").
    var trueAnomaly: Double { get }

}

public extension Orbit {

    /// The orbit radius relative to the center of the primary body.
    /// This parameter represents the scalar value of the radius.
    public var scalarRadius: Double {
        return length(radius)
    }

    /// The orbit velocity relative to the center of the primary body.
    /// This parameter represents the scalar value of the velocity.
    public var scalarVelocity: Double {
        return length(velocity)
    }

    /// The standard gravitational parameter (G * (M + m)
    ///
    /// The standard gravitational parameter μ of a celestial body is the
    /// product of the gravitational constant G and the mass M of the body.
    ///
    /// In the case of two-bodies orbiting each other, the standard
    /// gravitational parameter is given by the sum of each body standard
    /// gravitational parameter.
    public var standardGravitationalParameter: Double {
        return (primaryMass + secondaryMass) * G
    }

    /// The system reduced mass.
    ///
    /// The reduced mass is the "effective" inertial mass appearing in
    /// the two-body problem of Newtonian mechanics. It is a quantity
    /// which allows the two-body problem to be solved as if it were
    /// a one-body problem.
    public var reducedMass: Double {
        return 1 / (1 / primaryMass + 1 / secondaryMass)
    }

    /// The orbit mean angular motion
    ///
    /// The mean motion is the angular speed required for a body to
    /// complete one orbit, assuming constant speed in a circular orbit
    /// which completes in the same time as the variable speed,
    /// elliptical orbit of the actual body.
    public var meanAngularMotion: Double {
        return sqrt(standardGravitationalParameter / pow(semiMajorAxis, 3))
    }

    /// The orbit specific angular momentum vector
    ///
    /// The specific relative angular momentum is defined as the cross
    /// product of the relative position vector `r` and the relative
    /// velocity vector `v`
    public var specificAngularMomentum: Vector3<Double> {
        return cross(radius, velocity)
    }

    /// The orbit scalar specific angular momentum.
    ///
    /// The specific relative angular momentum is defined as the cross
    /// product of the relative position vector `r` and the relative
    /// velocity vector `v`
    ///
    /// Defined as the magnitude of `specificAngularMomentum`.
    public var scalarSpecificAngularMomentum: Double {
        return length(specificAngularMomentum)
    }

    /// The orbit angular momentum
    ///
    /// The angular momentum is the rotational analog of linear momentum.
    /// It is an important quantity in physics because it is a conserved
    /// quantity – the angular momentum of a system remains constant
    /// unless acted on by an external torque.
    public var angularMomentum: Vector3<Double> {
        return specificAngularMomentum * reducedMass
    }

    /// The orbit angular momentum
    ///
    /// The angular momentum is the rotational analog of linear momentum.
    /// It is an important quantity in physics because it is a conserved
    /// quantity – the angular momentum of a system remains constant
    /// unless acted on by an external torque.
    ///
    /// Defined as the magnitude of `angularMomentum`.
    public var scalarAngularMomentum: Double {
        return scalarSpecificAngularMomentum * reducedMass
    }

    /// The orbit specific orbital energy
    ///
    /// The specific orbital energy E of two orbiting bodies is the
    /// constant sum of their mutual potential energy and their total
    /// kinetic energy, divided by the reduced mass
    public var specificOrbitalEnergy: Double {
        return pow(scalarVelocity, 2) / 2 -
                standardGravitationalParameter / scalarRadius
    }

    /// The orbit orbital energy
    ///
    /// The specific orbital energy E of two orbiting bodies is the
    /// constant sum of their mutual potential energy and their total
    /// kinetic energy.
    public var orbitalEnergy: Double {
        return specificOrbitalEnergy * reducedMass
    }

    /// The orbit argument of latitude
    ///
    /// The argument of latitude is an angular parameter that defines the
    /// position of a body moving along a Kepler orbit. It is the angle
    /// between the ascending node and the body.
    public var argumentOfLatitude: Double {
        return atan(
                (radius.x * cos(longitudeOfAscedingNode) + radius.y * sin(longitudeOfAscedingNode)) /
                        (radius.z / sin(inclination))
        )
    }

    /// The orbit orbital period
    ///
    /// The orbital period is the time a given astronomical object takes
    /// to complete one orbit around another object, and applies in
    /// astronomy usually to planets or asteroids orbiting the Sun, moons
    /// orbiting planets, exoplanets orbiting other stars, or binary stars.
    public var orbitalPeriod: Double {
        return 2 * Double.pi * sqrt(pow(semiMajorAxis, 3) / standardGravitationalParameter)
    }

    /// The orbit eccentric anomaly
    ///
    /// The eccentric anomaly is an angular parameter that defines the
    /// position of a body that is moving along an elliptic Kepler orbit.
    /// The eccentric anomaly is one of three angular parameters
    /// ("anomalies") that define a position along an orbit, the other
    /// two being the true anomaly and the mean anomaly.
    public var eccentricAnomaly: Double {
        let v = trueAnomaly
        let e = eccentricity
        return 2 * atan((tan(v / 2) / sqrt(((1 + e) / (1 - e)))))
    }

    /// The orbit periapsis
    ///
    /// The point at which an orbiting object is closest to the center of
    /// mass of the body it is orbiting.
    public var periapsis: Vector3<Double> {
        let r = (1 - eccentricity) * semiMajorAxis
        return Vector3<Double>(
                x: r * (cos(longitudeOfAscedingNode) * cos(argumentOfPeriapsis + trueAnomaly) -
                        sin(longitudeOfAscedingNode) * sin(argumentOfPeriapsis + trueAnomaly) *
                                cos(inclination)),
                y: r * (sin(longitudeOfAscedingNode) * cos(argumentOfPeriapsis + trueAnomaly) +
                        cos(longitudeOfAscedingNode) * sin(argumentOfPeriapsis + trueAnomaly) *
                                cos(inclination)),
                z: r * (sin(inclination) * sin(argumentOfPeriapsis + trueAnomaly))
        )
    }

    /// The orbit apoapsis
    ///
    /// The point at which an orbiting object is farthest away from the center
    /// of mass of the body it is orbiting.
    public var apoapsis: Vector3<Double> {
        let r = (1 + eccentricity) * semiMajorAxis
        return Vector3<Double>(
                x: r * (cos(longitudeOfAscedingNode) * cos(argumentOfPeriapsis + trueAnomaly) -
                        sin(longitudeOfAscedingNode) * sin(argumentOfPeriapsis + trueAnomaly) *
                                cos(inclination)),
                y: r * (sin(longitudeOfAscedingNode) * cos(argumentOfPeriapsis + trueAnomaly) +
                        cos(longitudeOfAscedingNode) * sin(argumentOfPeriapsis + trueAnomaly) *
                                cos(inclination)),
                z: r * (sin(inclination) * sin(argumentOfPeriapsis + trueAnomaly))
        )
    }
}

public extension Orbit {
    /// The orbit time to periaspse passage
    ///
    /// Defines the amount of time until reaching (or after reaching)
    /// the orbit periapsis point.
    ///
    /// - Parameter at: computes the amount of time until reaching
    ///     persiapsis given that we `at` the given time.
    public func timeOfPeriapsisPassage(at time: Double) -> Double {
        let EA = eccentricAnomaly
        return time - (EA - eccentricity * sin(EA)) / meanAngularMotion
    }

    /// Computes the orbit mean anomaly at `time` considering the current
    /// orbital parameters are at `from`.
    ///
    /// The mean anomaly is an angle used in calculating the position of
    /// a body in an elliptical orbit in the classical two-body problem.
    /// It is the angular distance from the pericenter which a fictitious
    /// body would have if it moved in a circular orbit, with constant
    /// speed, in the same orbital period as the actual body in its
    /// elliptical orbit.
    ///
    /// - Parameter from: assumes the current orbit parameter are for
    ///     the time given at `from`.
    /// - Parameter to: calculates the mean anomaly for when the orbit
    ///     is at `time`.
    public func meanAnomaly(from: Double, to time: Double) -> Double {
        let T = timeOfPeriapsisPassage(at: from)
        return meanAngularMotion * (time - T)
    }

    /// Computes the orbit eccentric anomaly at `time` considering the
    /// current orbital parameters are at `from`. The parameter is
    /// computed with a maximum error of up to `maximumError`.
    ///
    /// The eccentric anomaly is an angular parameter that defines the
    /// position of a body that is moving along an elliptic Kepler orbit.
    /// The eccentric anomaly is one of three angular parameters
    /// ("anomalies") that define a position along an orbit, the other
    /// two being the true anomaly and the mean anomaly.
    ///
    /// - Parameter from: assumes the current orbit parameter are for
    ///     the time given at `from`.
    /// - Parameter to: calculates the eccentric anomaly for when the
    ///     orbit is at `time`.
    /// - Parameter maximumError: the maximum error to be accepted
    public func eccentricAnomaly(from: Double, to time: Double, maximumError: Double = 1e-8) -> Double {
        let M = meanAnomaly(from: from, to: time)
        let e = eccentricity

        if e == 1.0 {
            return pow(6 * M, 1.0 / 3.0)
        } else {
            var E = M / (1 - e)
            var E1 = E

            E = E1 - (M - E + e * sin(E)) / (e * cos(E) - 1)
            while abs(E - E1) > maximumError {
                E1 = E
                E = E1 - (M - E + e * sin(E)) / (e * cos(E) - 1)
            }

            return E
        }
    }

    /// Computes the orbit true anomaly at `time` considering the
    /// current orbital parameters are at `from`.
    ///
    /// Defines the position of the orbiting body along the ellipse at a
    /// specific time (the "epoch").
    ///
    /// - Parameter from: assumes the current orbit parameter are for
    ///     the time given at `from`.
    /// - Parameter to: calculates the true anomaly for when the orbit
    ///     is at `time`.
    public func trueAnomaly(from: Double, to time: Double) -> Double {
        let e = eccentricity
        let ea = eccentricAnomaly(from: from, to: time)

        let a1 = sqrt((1 + e) / (1 - e))
        let a2 = tan(ea / 2)

        return 2 * atan(a1 * a2)
    }

}

/// Represents a orbit at a given position from its cartesian radius (from the
/// center of the primary body) and orbital velocity vectors. These parameters
/// are known as "Orbital State Vectors".
///
/// The orbital state vectors (sometimes state vectors) of an orbit are cartesian
/// vectors of position (`radius`) and velocity (`velocity`) that together with
/// their time (epoch) uniquely determine the trajectory of the orbiting body in
/// space.
public struct CartesianOrbit: Orbit {

    /// The orbit primary body mass
    public var primaryMass: Double

    /// The orbit secondary body mass
    public var secondaryMass: Double

    /// The orbit radius relative to the center of the primary body
    public var radius: Vector3<Double>

    /// The orbit velocity relative to the center of the primary body
    public var velocity: Vector3<Double>

    /// Creates a new orbit from it's Cartesian representation.
    ///
    /// The created Cartesian orbit can be used to compute Keplerian,
    /// parameters.
    ///
    /// - Parameter primaryMass: the mass of the primary body
    /// - Parameter secondaryMass: the mass of the secondary body
    /// - Parameter radius: the current orbital radius
    /// - Parameter velocity: the current orbital velocity
    public init(primaryMass: Double, secondaryMass: Double,
                radius: Vector3<Double>, velocity: Vector3<Double>) {
        self.primaryMass = primaryMass
        self.secondaryMass = secondaryMass
        self.radius = radius
        self.velocity = velocity
    }

    /// The orbit specific angular momentum vector
    ///
    /// The specific relative angular momentum is defined as the cross
    /// product of the relative position vector `r` and the relative
    /// velocity vector `v`
    public var specificAngularMomentum: Vector3<Double> {
        return cross(radius, velocity)
    }

    /// The orbit ascending node
    ///
    /// The ascending node (or north node) is where the orbiting object
    /// moves north through the plane of reference.
    public var nodeVector: Vector3<Double> {
        return cross(Vector3(0, 0, 1), specificAngularMomentum)
    }

    /// The orbit eccentricity
    ///
    /// The shape of the ellipse, describing how much it is elongated
    /// compared to a circle
    ///
    /// This value represents the eccentricity as a vector.
    public var eccentricityVector: Vector3<Double> {
        let a = (pow(scalarVelocity, 2) - standardGravitationalParameter / scalarRadius) * radius
        let b = dot(radius, velocity) * velocity
        return (a - b) / (standardGravitationalParameter)
    }

    /// The orbit eccentricity
    ///
    /// The shape of the ellipse, describing how much it is elongated
    /// compared to a circle
    public var eccentricity: Double {
        return length(eccentricityVector)
    }

    /// The orbit semi-major axis
    ///
    /// The sum of the periapsis and apoapsis distances divided by
    /// two. For circular orbits, the semimajor axis is the distance
    /// between the centers of the bodies, not the distance of the bodies
    /// from the center of mass. For a parabolic orbit, this is infinite.
    /// For a hyperbolic orbit, if you graph the two-branched hyperbola on
    /// the Cartesian plane, is half the distance between periapsis and
    /// the opposing vertex and is usually treated as negative for orbital
    /// energy purposes.
    public var semiMajorAxis: Double {
        if eccentricity != 1.0 {
            return -(standardGravitationalParameter) / (2 * specificOrbitalEnergy)
        } else {
            return Double.infinity
        }
    }

    /// The orbit inclination
    ///
    /// The vertical tilt of the ellipse with respect to the reference
    /// plane, measured at the ascending node (where the orbit passes
    /// upward through the reference plane). Tilt angle is measured
    /// perpendicular to line of intersection between orbital plane
    /// and reference plane. Any three points on an ellipse will define
    /// the ellipse orbital plane. The plane and the ellipse are both
    /// two-dimensional objects defined in three-dimensional space.
    public var inclination: Double {
        return acos(specificAngularMomentum.z / scalarSpecificAngularMomentum)
    }

    /// The orbit longitude of the ascending node
    ///
    /// Horizontally orients the ascending node of the ellipse (where
    /// the orbit passes upward through the reference plane) with respect
    /// to the reference frame's vernal point.
    public var longitudeOfAscedingNode: Double {
        if nodeVector.y < 0 {
            return 2 * Double.pi - acos(nodeVector.x / length(nodeVector))
        } else {
            if length(nodeVector) == 0 {
                return 0.0
            }
            return acos(nodeVector.x / length(nodeVector))
        }
    }

    /// The orbit argument of periapsis
    ///
    /// Defines the orientation of the ellipse in the orbital plane,
    /// as an angle measured from the ascending node to the periapsis
    /// (the closest point the satellite object comes to the primary
    /// object around which it orbits).
    public var argumentOfPeriapsis: Double {
        if eccentricityVector.z < 0 {
            return 2 * Double.pi - acos(dot(eccentricityVector, nodeVector) / (eccentricity * length(nodeVector)))
        } else {
            if length(nodeVector) == 0 {
                return 0.0
            }
            return acos(dot(eccentricityVector, nodeVector) / (eccentricity * length(nodeVector)))
        }
    }

    /// The orbit true anomaly
    ///
    /// Defines the position of the orbiting body along the ellipse at a
    /// specific time (the "epoch").
    public var trueAnomaly: Double {
        if dot(radius, velocity) < 0 {
            return 2 * Double.pi -
                    acos(
                            dot(eccentricityVector, radius) /
                                    (length(eccentricityVector) * scalarRadius))
        } else {
            return acos(
                    dot(eccentricityVector, radius) /
                            (length(eccentricityVector) * scalarRadius))
        }
    }

}

/// Represents a orbit at a given it's orbital parameters (or elements). The orbital
/// elements are:
///
/// - `semiMajorAxis`: The sum of the periapsis and apoapsis distances divided by
///     two.
/// - `eccentricity`: The shape of the ellipse, describing how much it is elongated
///     compared to a circle.
/// - `inclination`: The vertical tilt of the ellipse with respect to the reference
///     plane, measured at the ascending node.
/// - `longitudeOfAscedingNode`: Horizontally orients the ascending node of the
///     ellipse with respect to the reference frame's vernal point.
/// - `argumentOfPeriapsis`: Defines the orientation of the ellipse in the orbital
///     plane, as an angle measured from the ascending node to the periapsis.
/// - `trueAnomaly`: dDefines the position of the orbiting body along the ellipse
///     at a specific time (the "epoch").
///
/// The Orbital elements are the parameters required to uniquely identify a specific
/// orbit. In celestial mechanics these elements are generally considered in classical
/// two-body systems, where a Kepler orbit is used.
public struct KeplerOrbit: Orbit {

    // MARK: - System parameters

    /// The orbit primary body mass
    public var primaryMass: Double

    /// The orbit secondary body mass
    public var secondaryMass: Double

    // MARK: - Keplerian parameters

    /// The orbit semi-major axis
    ///
    /// The sum of the periapsis and apoapsis distances divided by
    /// two. For circular orbits, the semimajor axis is the distance
    /// between the centers of the bodies, not the distance of the bodies
    /// from the center of mass. For a parabolic orbit, this is infinite.
    /// For a hyperbolic orbit, if you graph the two-branched hyperbola on
    /// the Cartesian plane, is half the distance between periapsis and
    /// the opposing vertex and is usually treated as negative for orbital
    /// energy purposes.
    public var semiMajorAxis: Double

    /// The orbit eccentricity
    ///
    /// The shape of the ellipse, describing how much it is elongated
    /// compared to a circle
    public var eccentricity: Double

    /// The orbit inclination
    ///
    /// The vertical tilt of the ellipse with respect to the reference
    /// plane, measured at the ascending node (where the orbit passes
    /// upward through the reference plane). Tilt angle is measured
    /// perpendicular to line of intersection between orbital plane
    /// and reference plane. Any three points on an ellipse will define
    /// the ellipse orbital plane. The plane and the ellipse are both
    /// two-dimensional objects defined in three-dimensional space.
    public var inclination: Double

    /// The orbit longitude of the ascending node
    ///
    /// Horizontally orients the ascending node of the ellipse (where
    /// the orbit passes upward through the reference plane) with respect
    /// to the reference frame's vernal point.
    public var longitudeOfAscedingNode: Double

    /// The orbit argument of periapsis
    ///
    /// Defines the orientation of the ellipse in the orbital plane,
    /// as an angle measured from the ascending node to the periapsis
    /// (the closest point the satellite object comes to the primary
    /// object around which it orbits).
    public var argumentOfPeriapsis: Double

    /// The orbit true anomaly
    ///
    /// Defines the position of the orbiting body along the ellipse at a
    /// specific time (the "epoch").
    public var trueAnomaly: Double

    /// Creates a new orbit from it's Kepler orbital parameters.
    ///
    /// The created orbit can be used to compute its rectangular (or cartesian)
    /// parameters.
    ///
    /// - Parameter primaryMass:                the primary body mass
    /// - Parameter secondaryMass:              the secondary body mass
    /// - Parameter semiMajorAxis:              the semi-major axis
    /// - Parameter eccentricity:               the eccentricity
    /// - Parameter inclination:                the inclination
    /// - Parameter longitudeOfAscedingNode:    the longitude of the ascending node
    /// - Parameter argumentOfPeriapsis:        the argument of periapsis
    /// - Parameter trueAnomaly:                the true anomaly
    public init(primaryMass: Double, secondaryMass: Double,
                semiMajorAxis: Double, eccentricity: Double, inclination: Double,
                ascendingNode: Double, argumentOfPeriapsis: Double,
                trueAnomaly: Double = 0) {
        self.primaryMass = primaryMass
        self.secondaryMass = secondaryMass
        self.semiMajorAxis = semiMajorAxis
        self.eccentricity = eccentricity
        self.inclination = inclination
        self.longitudeOfAscedingNode = ascendingNode
        self.argumentOfPeriapsis = argumentOfPeriapsis
        self.trueAnomaly = trueAnomaly
    }

    /// Creates a new orbit from it's Kepler orbital parameters.
    ///
    /// The created orbit can be used to compute its rectangular (or cartesian)
    /// parameters.
    ///
    /// - Parameter primaryMass:                the primary body mass
    /// - Parameter secondaryMass:              the secondary body mass
    /// - Parameter semiMajorAxis:              the semi-major axis
    /// - Parameter eccentricity:               the eccentricity
    /// - Parameter inclination:                the inclination
    /// - Parameter longitudeOfAscedingNode:    the longitude of the ascending
    ///                                         node
    /// - Parameter argumentOfPeriapsis:        the argument of periapsis
    /// - Parameter trueAnomaly:                the true anomaly
    /// - Parameter at:                         the time to base the
    ///                                         `trueAnomaly` from
    public init(primaryMass: Double, secondaryMass: Double,
                semiMajorAxis: Double, eccentricity: Double, inclination: Double,
                ascendingNode: Double, argumentOfPeriapsis: Double,
                trueAnomaly: Double, at epoch: Double) {
        self.primaryMass = primaryMass
        self.secondaryMass = secondaryMass
        self.semiMajorAxis = semiMajorAxis
        self.eccentricity = eccentricity
        self.inclination = inclination
        self.longitudeOfAscedingNode = ascendingNode
        self.argumentOfPeriapsis = argumentOfPeriapsis
        self.trueAnomaly = trueAnomaly
        self.trueAnomaly = self.trueAnomaly(from: epoch, to: 0)
    }

    // MARK: - Cartesian parameters

    /// The orbit radius relative to the center of the primary body
    public var radius: Vector3<Double> {
        let r = scalarRadius
        let o = Vector3<Double>(
                x: r * cos(trueAnomaly),
                y: r * sin(trueAnomaly),
                z: 0
        )
        return transform * o;
    }

    /// The orbit radius relative to the center of the primary body.
    /// This parameter represents the scalar value of the radius.
    public var scalarRadius: Double {
        return pow(scalarSpecificAngularMomentum, 2) /
                (standardGravitationalParameter * (1 + eccentricity * cos(trueAnomaly)))
    }

    /// The orbit velocity relative to the center of the primary body
    public var velocity: Vector3<Double> {
//        let ea = eccentricAnomaly
//        let e = eccentricity
//        let r = scalarRadius
//        let h = scalarSpecificAngularMomentum
//
//        let o = (h / r) * Vector3<Double>(
//                x: -sin(ea),
//                y: sqrt(1 - pow(e, 2)) * cos(ea),
//                z: 0
//        )
//
//        return transform * o;

        let r = scalarRadius
        let h = scalarSpecificAngularMomentum
        let R = radius
        let p = semiMajorAxis * (1 - pow(eccentricity, 2))

        return Vector3<Double>(
                x: R.x * h * eccentricity * sin(trueAnomaly) / (r * p) - (h / r) * (
                        cos(longitudeOfAscedingNode) * sin(argumentOfPeriapsis + trueAnomaly) +
                                sin(longitudeOfAscedingNode) * cos(argumentOfPeriapsis + trueAnomaly) * cos(inclination)),
                y: R.y * h * eccentricity * sin(trueAnomaly) / (r * p) - (h / r) * (
                        sin(longitudeOfAscedingNode) * sin(argumentOfPeriapsis + trueAnomaly) -
                                cos(longitudeOfAscedingNode) * cos(argumentOfPeriapsis + trueAnomaly) * cos(inclination)),
                z: R.z * h * eccentricity * sin(trueAnomaly) / (r * p) + (h / r) * (
                        sin(inclination) * cos(argumentOfPeriapsis + trueAnomaly))
        )
    }

    /// The orbit scalar specific angular momentum.
    ///
    /// Defined as the magnitude of `specificAngularMomentum`.
    public var scalarSpecificAngularMomentum: Double {
        return sqrt(standardGravitationalParameter * semiMajorAxis * (1 - pow(eccentricity, 2)))
    }

    private var transform: Matrix3x3<Double> {
        var transform = Matrix4x4<Double>()
        transform = SGLMath.rotate(transform, longitudeOfAscedingNode, Vector3(0, 0, 1))
        transform = SGLMath.rotate(transform, inclination, Vector3(1, 0, 0))
        transform = SGLMath.rotate(transform, argumentOfPeriapsis, Vector3(0, 0, 1))
        return Matrix3x3(transform)
    }

}

/// Represents a static and precomputed orbit. These orbits have all their parameters
/// "hard coded" and cannot perform recalculation of parameters, but are very fast
/// since their parameters do not have to be dynamically computed each time they are
/// required.
public struct PrecomputedOrbit: Orbit {

    // MARK: - System parameters

    /// The orbit primary body mass
    public let primaryMass: Double

    /// The orbit secondary body mass
    public let secondaryMass: Double

    // MARK: - Cartesian parameters

    /// The orbit radius relative to the center of the primary body
    public let radius: Vector3<Double>

    /// The orbit radius relative to the center of the primary body.
    /// This parameter represents the scalar value of the radius.
    public let scalarRadius: Double

    /// The orbit velocity relative to the center of the primary body
    public let velocity: Vector3<Double>

    /// The orbit velocity relative to the center of the primary body.
    /// This parameter represents the scalar value of the velocity.
    public let scalarVelocity: Double

    // MARK: - Keplerian parameters

    /// The orbit semi-major axis
    ///
    /// The sum of the periapsis and apoapsis distances divided by
    /// two. For circular orbits, the semimajor axis is the distance
    /// between the centers of the bodies, not the distance of the bodies
    /// from the center of mass. For a parabolic orbit, this is infinite.
    /// For a hyperbolic orbit, if you graph the two-branched hyperbola on
    /// the Cartesian plane, is half the distance between periapsis and
    /// the opposing vertex and is usually treated as negative for orbital
    /// energy purposes.
    public let semiMajorAxis: Double

    /// The orbit eccentricity
    ///
    /// The shape of the ellipse, describing how much it is elongated
    /// compared to a circle
    public let eccentricity: Double

    /// The orbit inclination
    ///
    /// The vertical tilt of the ellipse with respect to the reference
    /// plane, measured at the ascending node (where the orbit passes
    /// upward through the reference plane). Tilt angle is measured
    /// perpendicular to line of intersection between orbital plane
    /// and reference plane. Any three points on an ellipse will define
    /// the ellipse orbital plane. The plane and the ellipse are both
    /// two-dimensional objects defined in three-dimensional space.
    public let inclination: Double

    /// The orbit longitude of the ascending node
    ///
    /// Horizontally orients the ascending node of the ellipse (where
    /// the orbit passes upward through the reference plane) with respect
    /// to the reference frame's vernal point.
    public let longitudeOfAscedingNode: Double

    /// The orbit argument of periapsis
    ///
    /// Defines the orientation of the ellipse in the orbital plane,
    /// as an angle measured from the ascending node to the periapsis
    /// (the closest point the satellite object comes to the primary
    /// object around which it orbits).
    public let argumentOfPeriapsis: Double

    /// The orbit true anomaly
    ///
    /// Defines the position of the orbiting body along the ellipse at a
    /// specific time (the "epoch").
    public let trueAnomaly: Double

    /// The orbit mean angular motion
    ///
    /// The mean motion is the angular speed required for a body to
    /// complete one orbit, assuming constant speed in a circular orbit
    /// which completes in the same time as the variable speed,
    /// elliptical orbit of the actual body.
    public let meanAngularMotion: Double

    /// The orbit specific angular momentum vector
    ///
    /// The specific relative angular momentum is defined as the cross
    /// product of the relative position vector `r` and the relative
    /// velocity vector `v`
    public let specificAngularMomentum: Vector3<Double>

    /// The orbit scalar specific angular momentum.
    ///
    /// Defined as the magnitude of `specificAngularMomentum`.
    public let scalarSpecificAngularMomentum: Double


    /// The orbit angular momentum
    ///
    /// The angular momentum is the rotational analog of linear momentum.
    /// It is an important quantity in physics because it is a conserved
    /// quantity – the angular momentum of a system remains constant
    /// unless acted on by an external torque.
    public let angularMomentum: Vector3<Double>

    /// The orbit angular momentum
    ///
    /// The angular momentum is the rotational analog of linear momentum.
    /// It is an important quantity in physics because it is a conserved
    /// quantity – the angular momentum of a system remains constant
    /// unless acted on by an external torque.
    ///
    /// Defined as the magnitude of `angularMomentum`.
    public let scalarAngularMomentum: Double

    /// The orbit specific energy
    ///
    /// The specific orbital energy E of two orbiting bodies is the
    /// constant sum of their mutual potential energy and their total
    /// kinetic energy, divided by the reduced mass
    public let specificOrbitalEnergy: Double

    /// The orbit orbital energy
    ///
    /// The specific orbital energy E of two orbiting bodies is the
    /// constant sum of their mutual potential energy and their total
    /// kinetic energy.
    public let orbitalEnergy: Double

    /// The orbit argument of latitude
    ///
    /// The argument of latitude is an angular parameter that defines the
    /// position of a body moving along a Kepler orbit. It is the angle
    /// between the ascending node and the body.
    public let argumentOfLatitude: Double

    /// The orbit orbital period
    ///
    /// The orbital period is the time a given astronomical object takes
    /// to complete one orbit around another object, and applies in
    /// astronomy usually to planets or asteroids orbiting the Sun, moons
    /// orbiting planets, exoplanets orbiting other stars, or binary stars.
    public let orbitalPeriod: Double

    /// The orbit eccentric anomaly
    ///
    /// The eccentric anomaly is an angular parameter that defines the
    /// position of a body that is moving along an elliptic Kepler orbit.
    /// The eccentric anomaly is one of three angular parameters
    /// ("anomalies") that define a position along an orbit, the other
    /// two being the true anomaly and the mean anomaly.
    public let eccentricAnomaly: Double

    /// The orbit periapsis
    ///
    /// The point at which an orbiting object is closest to the center of
    /// mass of the body it is orbiting.
    public let periapsis: Vector3<Double>

    /// The orbit apoapsis
    ///
    /// The point at which an orbiting object is farthest away from the center
    /// of mass of the body it is orbiting.
    public let apoapsis: Vector3<Double>

    /// Creates a new precomputed orbit by precomputing all parameters of
    /// the given orbit. Once a precomputed orbit is created, its parameter
    /// cannot be changed.
    ///
    /// - Parameter orbit: the orbit to be used as base for the precomputed
    ///     parameters.
    public init(_ orbit: Orbit) {
        primaryMass = orbit.primaryMass
        secondaryMass = orbit.secondaryMass

        radius = orbit.radius
        scalarRadius = orbit.scalarRadius
        velocity = orbit.velocity
        scalarVelocity = orbit.scalarVelocity

        semiMajorAxis = orbit.semiMajorAxis
        eccentricity = orbit.eccentricity
        inclination = orbit.inclination
        longitudeOfAscedingNode = orbit.longitudeOfAscedingNode
        argumentOfPeriapsis = orbit.argumentOfPeriapsis
        trueAnomaly = orbit.trueAnomaly

        meanAngularMotion = orbit.meanAngularMotion
        specificAngularMomentum = orbit.specificAngularMomentum
        scalarSpecificAngularMomentum = orbit.scalarSpecificAngularMomentum
        angularMomentum = orbit.angularMomentum
        scalarAngularMomentum = orbit.scalarAngularMomentum
        specificOrbitalEnergy = orbit.specificOrbitalEnergy
        orbitalEnergy = orbit.orbitalEnergy
        argumentOfLatitude = orbit.argumentOfLatitude
        orbitalPeriod = orbit.orbitalPeriod
        eccentricAnomaly = orbit.eccentricAnomaly
        periapsis = orbit.periapsis
        apoapsis = orbit.apoapsis
    }

}

public extension Orbit {

    /// Creates a new orbit by simulating the orbit's position as it
    /// would be at `time` seconds since its epoch.
    ///
    /// - Parameter time: the time to simulate the orbit at
    /// - Returns: a new orbit with all parameters set at the given
    ///     time.
    public subscript(at time: Double) -> Orbit {
        return KeplerOrbit(
                primaryMass: self.primaryMass,
                secondaryMass: self.secondaryMass,
                semiMajorAxis: self.semiMajorAxis,
                eccentricity: self.eccentricity,
                inclination: self.inclination,
                ascendingNode: self.longitudeOfAscedingNode,
                argumentOfPeriapsis: self.argumentOfPeriapsis,
                trueAnomaly: self.trueAnomaly(from: 0, to: time))
    }

    /// Creates a new orbit by changing its `trueAnomaly`.
    ///
    /// - Parameter trueAnomaly: the value of `trueAnomaly` to simulate
    ///     the orbit at.
    /// - Returns: a new orbit with the `trueAnomaly` set at the given
    ///     value.
    public subscript(trueAnomaly trueAnomaly: Double) -> Orbit {
        return KeplerOrbit(
                primaryMass: self.primaryMass,
                secondaryMass: self.secondaryMass,
                semiMajorAxis: self.semiMajorAxis,
                eccentricity: self.eccentricity,
                inclination: self.inclination,
                ascendingNode: self.longitudeOfAscedingNode,
                argumentOfPeriapsis: self.argumentOfPeriapsis,
                trueAnomaly: trueAnomaly)
    }

    /// Creates a new orbit by offsetting the current radius by `deltaRadius`
    ///
    /// - Parameter deltaRadius: the amount of radius to be added to the
    ///     existing orbit.
    /// - Returns: a new orbit with the `radius` parameter increased by
    ///     `deltaRadius`.
    public subscript(deltaRadius deltaRadius: Vector3<Double>) -> Orbit {
        return self[deltaRadius: deltaRadius, deltaVelocity: Vector3(0)]
    }

    /// Creates a new orbit by offsetting its current velocity by `deltaVelocity`
    ///
    /// - Parameter deltaVelocity: the amount of velocity to be added to the
    ///     existing orbit.
    /// - Returns: a new orbit with the `velocity` parameter increased by
    ///     `deltaVelocity`.
    public subscript(deltaVelocity deltaVelocity: Vector3<Double>) -> Orbit {
        return self[deltaRadius: Vector3(0), deltaVelocity: deltaVelocity]
    }

    /// Creates a new orbit by offsetting the current radius by `deltaRadius`,
    /// and its velocity by `deltaVelocity`
    ///
    /// - Parameter deltaRadius: the amount of radius to be added to the
    ///     existing orbit.
    /// - Parameter deltaVelocity: the amount of velocity to be added to the
    ///     existing orbit.
    /// - Returns: a new orbit with the `radius` parameter increased by
    ///     `deltaRadius` and `velocity` parameter increased by `deltaVelocity`.
    public subscript(deltaRadius deltaRadius: Vector3<Double>,
                     deltaVelocity deltaVelocity: Vector3<Double>) -> Orbit {
        return CartesianOrbit(
                primaryMass: self.primaryMass,
                secondaryMass: self.secondaryMass,
                radius: self.radius + deltaRadius,
                velocity: self.velocity + deltaVelocity)
    }

}

public extension Orbit {

    /// Returns a new precomputed orbit
    public var precomputed: PrecomputedOrbit {
        if let precomputed = self as? PrecomputedOrbit {
            return precomputed
        }
        return PrecomputedOrbit(self)
    }

}
