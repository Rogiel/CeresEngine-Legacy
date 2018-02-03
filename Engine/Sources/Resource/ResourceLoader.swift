//
// Created by Rogiel Sulzbach on 10/30/17.
//

public protocol ResourceLoader {

    func supports<T>(resource: Resource, ofType type: T.Type) -> Bool

    /// Loads the resource
    func load<T>(resource: Resource) throws -> T

}
