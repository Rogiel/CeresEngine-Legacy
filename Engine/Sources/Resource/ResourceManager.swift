//
// Created by Rogiel Sulzbach on 10/30/17.
//

import Foundation
import Dispatch

public class ResourceManager {

    /// The path that points to the root of the Resources directory
    public var resourcesPath: String = ""

    /// A list of supported resource loaders
    public var resourceLoaders: [ResourceLoader] = [
        STBImageLoader(),
        OBJMeshLoader()
//        JSONModelLoader()
    ]

    /// A dictionary of known resources by name
    private var resources: [String: WeakResource] = [:]

    /// Creates a new ResourceManager
    public init(resourcesPath: String) {
        self.resourcesPath = resourcesPath
    }

    /// Creates a new ResourceManager
    public convenience init() {
        self.init(resourcesPath: FileManager.default.currentDirectoryPath)
    }

    /// Gets a resource by name
    public func resource(named name: String) throws -> FileResource {
        let path = "\(resourcesPath)/\(name)"
        if FileManager.default.fileExists(atPath: path) {
            return FileResource(resourceManager: self, URL(fileURLWithPath: path))
        }

        throw ResourceLoadingError.noSuchResource
    }

    public func load<T>(named name: String) throws -> T {
        let resource = try self.resource(named: name)
        for loader in self.resourceLoaders {
            if loader.supports(resource: resource, ofType: T.self) {
                return try loader.load(resource: resource)
            }
        }
        throw ResourceLoadingError.noLoader
    }

    /// A queue used to load resources asynchronously
    public var loaderQueue = DispatchQueue(label: "ResourceManager Async Loader")

    /// The default application resource manager
    public static var `default`: ResourceManager = ResourceManager()

    /// A wrapper that holds a weak reference to the resource
    private struct WeakResource {
        /// A weak reference to a resource
        var resource: Resource?
    }
}

public enum ResourceLoadingError: Error {
    /// A error thrown if there is no loader capable of loading the resource
    /// kind
    case noLoader
    case noSuchResource
    case invalidFormat
    case invalidResourceType
}
