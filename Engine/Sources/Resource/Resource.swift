//
// Created by Rogiel Sulzbach on 10/30/17.
//

import Foundation
import Dispatch

public protocol Resource {

    /// The resource URL
    var url: URL { get }

    /// Opens a new InputStream to the resource
    func openInputStream() -> InputStream?

    /// The resource data
    var data: Data? { get }

    /// The resource manager that is currently managing this or a parent of
    /// this resource.
    var resourceManager: ResourceManager { get }

}

public class FileResource: Resource {

    /// The resource URL
    public let url: URL

    /// The resource manager that is currently managing this or a parent of
    /// this resource.
    public var resourceManager: ResourceManager

    /// Creates a new FileResource from a `url` object.
    public init(resourceManager: ResourceManager, _ url: URL) {
        self.resourceManager = resourceManager
        self.url = url
    }

    /// Opens a new InputStream to the resource
    public func openInputStream() -> InputStream? {
        return InputStream(url: url)
    }

    /// The resource data
    public var data: Data? {
        return try? Data(contentsOf: url)
    }

}
