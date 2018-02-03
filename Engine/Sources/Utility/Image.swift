//
//  Image.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import Foundation
import SGLImage
import CGameEngine

public protocol Image {

    var width: Int { get }
    var height: Int { get }
    var channels: Int { get }

    /// Executes the `body` with a mutable buffer pointer for the image
    func withUnsafeMutableBufferPointer(_ body: (UnsafeMutableBufferPointer<UInt8>) throws -> Void) rethrows

}

public class STBImageLoader: ResourceLoader {
    public func supports<T>(resource: Resource, ofType type: T.Type) -> Bool {
        return type == Image.self &&
                (resource.url.pathExtension == "jpg" ||
                        resource.url.pathExtension == "png")
    }

    /// Loads the resource
    public func load<T>(resource: Resource) throws -> T {
        guard let resource = resource as? FileResource else {
            throw ResourceLoadingError.invalidResourceType
        }

        print("Loading \(resource.url.path)")
        stbi_set_flip_vertically_on_load(Int32(truncating: true))
        if let image = ImageRGB(resource: resource) {
            return image as! T
        } else if let image = ImageRGBA(resource: resource) {
            return image as! T
        }
        throw ResourceLoadingError.invalidFormat
    }
}

public class ImageRGB: Image {

    /// The backing image
    var backingImage: Data

    public var width: Int
    public var height: Int
    public var channels: Int {
        return 3
    }

    /// Creates a new RGB image with a `backingImage`.
    public convenience init?(resource: FileResource) {
        self.init(path: resource.url.path)
    }

    /// Creates a new RGB image with a `backingImage`.
    public init?(path: String) {
        guard let file = fopen(path, "rb") else {
            return nil
        }
        defer {
            fclose(file)
        }

        var width: Int32 = 0
        var height: Int32 = 0
        var channels: Int32 = 0

        guard let loaded = stbi_load_from_file(file, &width, &height, &channels, 3) else {
            return nil
        }
        defer {
            stbi_image_free(loaded)
        }

        backingImage = loaded.withMemoryRebound(to: UInt8.self, capacity: Int(width * height * channels)) { (ptr) in
            return Data(bytes: ptr, count: Int(width * height * channels))
        }

        self.width = Int(width)
        self.height = Int(height)
    }

    /// Executes the `body` with a mutable buffer pointer for the image
    public func withUnsafeMutableBufferPointer(_ body: (UnsafeMutableBufferPointer<UInt8>) throws -> Void) rethrows {
        try backingImage.withUnsafeMutableBytes { (ptr) in
            try body(UnsafeMutableBufferPointer<UInt8>(start: ptr, count: Int(width * height * 3)))
        }
    }

}

public class ImageRGBA: Image {

    /// The backing image
    var backingImage: Data

    public var width: Int
    public var height: Int
    public var channels: Int {
        return 4
    }

    /// Creates a new RGB image with a `backingImage`.
    public convenience init?(resource: FileResource) {
        self.init(path: resource.url.path)
    }

    /// Creates a new RGB image with a `backingImage`.
    public init?(path: String) {
        let file = fopen(path, "rb")!
        defer {
            fclose(file)
        }

        var width: Int32 = 0
        var height: Int32 = 0
        var channels: Int32 = 0

        guard let loaded = stbi_load_from_file(file, &width, &height, &channels, 4) else {
            return nil
        }
        defer {
            stbi_image_free(loaded)
        }

        backingImage = loaded.withMemoryRebound(to: UInt8.self, capacity: Int(width * height * channels)) { (ptr) in
            return Data(bytes: ptr, count: Int(width * height * channels))
        }

        self.width = Int(width)
        self.height = Int(height)
    }

    /// Executes the `body` with a mutable buffer pointer for the image
    public func withUnsafeMutableBufferPointer(_ body: (UnsafeMutableBufferPointer<UInt8>) throws -> Void) rethrows {
        try backingImage.withUnsafeMutableBytes { (ptr) in
            try body(UnsafeMutableBufferPointer<UInt8>(start: ptr, count: Int(width * height * 4)))
        }
    }

}
