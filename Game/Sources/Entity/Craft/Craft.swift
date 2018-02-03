//
// Created by Rogiel Sulzbach on 10/13/17.
//

public class Craft {

    public weak var delegate: CraftDelegate? = nil

    /// A string representing a visible name for the craft
    public var name: String

    /// A enum representing the craft type
    public var type: CraftType = .none

//    public var resources: [Resource] = []
//    public var resourcesByName: [String: Resource] = [:]

    public init(named name: String) {
        self.name = name
    }

}

public protocol CraftDelegate : class { }
public extension CraftDelegate {
    func craft(_ craft: Craft, enteredSphereOfInfluence planet: Planet) {}
    func craft(_ craft: Craft, leftSphereOfInfluence planet: Planet) {}
    func craft(_ craft: Craft, explodedWithReason reason: CraftExplosionReason) {}

    func craft(_ craft: Craft, shouldDecouplePart part: CraftAssemblyPart) -> Bool { return true }
    func craft(_ craft: Craft, decoupledPart part: CraftAssemblyPart) {}

    func craft(_ craft: Craft, landedOnPlanet planet: Planet, landingParameters: CraftLandingParameters) {}
}

public struct CraftLandingParameters {

}
