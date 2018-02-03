//
// Created by Rogiel Sulzbach on 2/18/18.
//

import Foundation

enum ResourceLoadingError: Error {
    case unknown
}

class ResourceManager {
    var loaders: [ResourceLoader] = []

    func load<T>(resource: Resource) throws -> T {
        for loader in loaders {
            if loader.supports(resource: resource, ofType: T.self) {
                return loader.load(resource: resource)
            }
        }
        throw ResourceLoadingError.unknown
    }

}

struct Resource {

}

protocol ResourceLoader {
    func supports<T>(resource: Resource, ofType type: T.Type) -> Bool
    func load<T>(resource: Resource) -> T
}

protocol Image {

}

struct ConcreteImage : Image {

}

class ConcreteImageLoader : ResourceLoader {
    func supports<T>(resource: Resource, ofType type: T.Type) -> Bool {
        return (type as? ConcreteImage.Type) != nil
    }

    func load<T>(resource: Resource) -> T {
        return ConcreteImage() as! T
    }
}

class AnotherImageType: Image {

}

let manager = ResourceManager()
manager.loaders = [
    ConcreteImageLoader()
]

let resource = Resource()
let image: Image = try! manager.load(resource: resource)
