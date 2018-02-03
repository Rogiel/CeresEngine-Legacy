//
//  Vertex.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import SGLMath

public struct Vertex {

    /// The vertex position
    public var position: Vector3<Float>
    
    /// The vertex normal
    public var normal: Vector3<Float>

    /// The vertex texture coordinates
    public var texCoords: Vector2<Float>

    /// The vertex tagent
    public var tangent: Vector3<Float>
    
    public init(position: Vector3<Float>,
                normal: Vector3<Float>? = nil,
                texCoords: Vector2<Float>? = nil,
                tangent: Vector3<Float>? = nil) {
        self.position = position
        self.normal = normal ?? Vector3<Float>(0.0)
        self.texCoords = texCoords ?? Vector2<Float>(1.0)
        self.tangent = tangent ?? Vector3<Float>(0.0)
    }
    
}

public extension Vertex {

    public static var stride = MemoryLayout<Vertex>.stride

    public static var positionOffset = 0
    public static var normalOffset = positionOffset + MemoryLayout<Vector3<Float>>.stride
    public static var texCoordsOffset = normalOffset + MemoryLayout<Vector3<Float>>.stride
    public static var tangentOffset = texCoordsOffset + MemoryLayout<Vector2<Float>>.stride

}