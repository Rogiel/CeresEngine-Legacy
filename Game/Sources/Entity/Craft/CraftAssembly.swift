//
// Created by Rogiel Sulzbach on 10/13/17.
//

public class CraftAssembly : Codable {

    public var parts: [CraftAssemblyPart] = []

}

public class CraftAssemblyPart : Codable {

}

public extension CraftAssembly {

    private enum CodingKeys: String, CodingKey {
        case parts = "parts"
    }

}