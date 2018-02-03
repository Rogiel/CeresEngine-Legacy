import Game
import SGLMath
import Foundation

let auToM = 149597870.700*1000.0
let dayToS = 86400.0

let epoch: Double = 946684800
let the10th: Double = 1518220800 - epoch

func check(_ cartesianOrbit: CartesianOrbit, _ keplerOrbit: KeplerOrbit) {
    let nasaRadius = cartesianOrbit.radius
    let mineRadius = keplerOrbit.radius

    let nasaVelocity = cartesianOrbit.velocity
    let mineVelocity = keplerOrbit.velocity

    print("nasa = \(nasaRadius), \(nasaVelocity)")
    print("mine = \(mineRadius), \(mineVelocity)")

    print("error = \(nasaRadius - mineRadius) (\(((mineRadius - nasaRadius) / nasaRadius) * 100)%")
    print("error = \(nasaVelocity - mineVelocity) (\(((mineVelocity - nasaVelocity) / nasaVelocity) * 100)%")
    print("trueAnomaly = \(cartesianOrbit.trueAnomaly), \(keplerOrbit.trueAnomaly)")
}

//EC     Eccentricity, e
//QR     Periapsis distance, q (au)
//IN     Inclination w.r.t XY-plane, i (degrees)
//OM     Longitude of Ascending Node, OMEGA, (degrees)
//W      Argument of Perifocus, w (degrees)
//Tp     Time of periapsis (Julian Day Number)
//N      Mean motion, n (degrees/day)
//MA     Mean anomaly, M (degrees)
//TA     True anomaly, nu (degrees)
//A      Semi-major axis, a (au)
//AD     Apoapsis distance (au)
//PR     Sidereal orbit period (day)

//X =-7.646500467464322E-01 Y = 6.281152229299772E-01 Z =-1.401244125182029E-04
//VX=-1.113302204015399E-02 VY=-1.341711423939538E-02 VZ= 1.078102044540272E-06
let earthCartesianOrbit = CartesianOrbit(
        primaryMass: 1.989e30, secondaryMass: 5.9736e24,
        radius: Vector3(-7.646500467464322E-01, 6.281152229299772E-01, -1.401244125182029E-04) * auToM,
        velocity: Vector3(-1.113302204015399E-02, -1.341711423939538E-02, 1.078102044540272E-06) * auToM / dayToS)

//EC= 1.591338849796446E-02 QR= 9.887621786140570E-01 IN= 8.869311254690260E-03
//OM= 2.067704419359796E+02 W = 2.754279318663343E+02 Tp=  2458141.291291997768
//N = 9.792769168079074E-01 MA= 1.783136743132932E+01 TA= 1.840052559047124E+01
//A = 1.004751174396007E+00 AD= 1.020740170177957E+00 PR= 3.676181821720778E+02
var earthKeplerOrbit = KeplerOrbit(
        primaryMass: 1.989e30, secondaryMass: 5.9736e24,
        semiMajorAxis: 1.004751174396007E+00 * auToM,
        eccentricity: 1.591338849796446E-02,
        inclination: radians(8.869311254690260E-03),
        ascendingNode: radians(2.067704419359796E+02),
        argumentOfPeriapsis: radians(2.754279318663343E+02),
        trueAnomaly: radians(1.840052559047124E+01))
check(earthCartesianOrbit, earthKeplerOrbit)

let teslaCartesianOrbit = CartesianOrbit(
        primaryMass: 1250.0, secondaryMass: 1.989e30,
        radius: Vector3(-7.690802173107607E-01, 6.236475652855626E-01, -1.393643118679943E-03) * auToM,
        velocity: Vector3(-1.252512462814835E-02, -1.488196764720304E-02, -3.764542211631047E-04) * auToM / dayToS
)
var teslaKeplerOrbit = KeplerOrbit(
        primaryMass: 1250.0, secondaryMass: 1.989e30,
        semiMajorAxis: 1.346782739324727E+00 * auToM,
        eccentricity: 2.653803281862795E-01,
        inclination: radians(1.110391008725130E+00),
        ascendingNode: radians(3.167970783953779E+02),
        argumentOfPeriapsis: radians(1.791663064550042E+02),
        trueAnomaly: radians(4.998779818046527E+00))
check(teslaCartesianOrbit, teslaKeplerOrbit)

print(teslaKeplerOrbit.scalarRadius)

let distance1 = earthCartesianOrbit.radius - teslaCartesianOrbit.radius
let distance2 = earthKeplerOrbit.radius - teslaKeplerOrbit.radius

earthKeplerOrbit.trueAnomaly = earthKeplerOrbit.trueAnomaly(from: the10th, to: 0)
teslaKeplerOrbit.trueAnomaly = teslaKeplerOrbit.trueAnomaly(from: the10th, to: 0)

//let t: Double = Date().timeIntervalSince1970 - epoch
let t: Double = the10th
let earth = earthKeplerOrbit[at: t]
let roadster = teslaKeplerOrbit[at: t]

check(teslaCartesianOrbit, roadster as! KeplerOrbit)

let distance = length(earth.radius - roadster.radius)
print("d = \(distance / 1000) km")

let deltaVelocity = earth.velocity - roadster.velocity;

//print("\(length(distance1) / 1000) km, \(length(distance1) / 1000 / 3600 / 24 / 3) km per second")
//print(length(distance1) / length(deltaVelocity) / 3600 / 24)
print("\(length(deltaVelocity) / 1000) km per second (relative to earth)")
//print("\(length(teslaKeplerOrbit.velocity) / 1000) km per second (relative to sun)")

//let diff = length(distance1) - length(distance2)
//print(diff)

//X =-7.936304551587051E-01 Y = 5.935323459595009E-01 Z =-2.139477360552794E-03
//VX=-1.202594612452683E-02 VY=-1.523299013197811E-02 VZ=-3.701952255171635E-04
let teslaCartesianOrbit12th = CartesianOrbit(
        primaryMass: 1250.0, secondaryMass: 1.989e30,
        radius: Vector3(-7.936304551587051E-01, 5.935323459595009E-01, -2.139477360552794E-03) * auToM,
        velocity: Vector3(-1.202594612452683E-02, -1.523299013197811E-02, -3.701952255171635E-04) * auToM / dayToS
)
check(teslaCartesianOrbit12th, roadster as! KeplerOrbit)