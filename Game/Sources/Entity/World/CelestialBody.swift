//
// Created by Rogiel Sulzbach on 10/22/17.
//

import GameEngine

public protocol CelestialBody : Positioning {

    /// The celestial body radius
    var radius: Double { get }

    /// The celestial body mass
    var mass: Double { get }

}
