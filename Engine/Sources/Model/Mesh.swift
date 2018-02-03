//
//  Mesh.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import Foundation

public protocol Mesh {

//    /// A FileResource can return any other resource
//    typealias SubResource = VertexBuffer

    /// A list of vertices the compose a mesh
    var vertices: [Vertex] { get }

    /// A list of indices to the vertex array to form triangles
    var indices: [UInt32]? { get }

    /// The vertex buffer for the mesh
    var vertexBuffer: VertexBuffer? { get set }

    /// A list of vertex attributes that represent how the rendering engine
    /// should map the mesh memory data into individual vertex input in shaders.
    var vertexAttributes: [VertexAttribute] { get }

}

public protocol MutableMesh : Mesh {

    /// A list of vertices the compose a mesh
    var vertices: [Vertex] { get set }

    /// A list of indices to the vertex array to form triangles
    var indices: [UInt32]? { get set }

    /// The vertex buffer for the mesh
    var vertexBuffer: VertexBuffer? { get set }

}

public enum PrimitiveType {
    /// Draw each vertex as a single point
    case point

    /// Draw each vertex as a line strip
    case lineStrip

    /// Draw each vertex as a line in a loop
    case lineLoop

    /// Draw each vertex as a line
    case line

    /// Draw each vertex as a triangle strip
    case triangleStrip

    /// Draw each vertex as a triangle fan
    case triangleFan

    /// Draw each 3 vertices as triangle
    case triangle

    /// Draws each 3 vertices as quad strip
    case quadStrip

    /// Draw each 4 vertices as a quad
    case quad

    /// Draw each vertex as part of a polygon
    case polygon

    /// Draw a patch. A patch is a geometry composed of `verticesPerPatch`
    /// vertices.
    case patch(verticesPerPatch: Int)
}

public class SimpleMesh: MutableMesh {

    /// A list of vertices the compose a mesh
    public var vertices: [Vertex] = []

    /// A list of indices to the vertex array to form triangles
    public var indices: [UInt32]? = nil

    /// The vertex buffer for the mesh
    public var vertexBuffer: VertexBuffer?

    /// The resource subresource
    public var subresource: Any?

    public init(vertices: [Vertex], indices: [UInt32]? = nil) {
        self.vertices = vertices
        self.indices = indices
    }

    /// A list of vertex attributes that represent how the rendering engine
    /// should map the mesh memory data into individual vertex input in shaders.
    public var vertexAttributes: [VertexAttribute] {
        return [
            .position(.init(
                    size: 3,
                    type: Float.self,
                    normalize: false,
                    stride: MemoryLayout<Vertex>.stride,
                    offset: Vertex.positionOffset
            )),
            .normal(.init(
                    size: 3,
                    type: Float.self,
                    normalize: false,
                    stride: MemoryLayout<Vertex>.stride,
                    offset: Vertex.normalOffset
            )),
            .textureCoordinates(.init(
                    size: 2,
                    type: Float.self,
                    normalize: false,
                    stride: MemoryLayout<Vertex>.stride,
                    offset: Vertex.texCoordsOffset
            )),
            .tangent(.init(
                    size: 3,
                    type: Float.self,
                    normalize: false,
                    stride: MemoryLayout<Vertex>.stride,
                    offset: Vertex.tangentOffset
            ))
        ]
    }

}

public extension Mesh {

    public func eachFace(_ body: ([Vertex]) throws -> Void) rethrows {
        if let indices = self.indices {
            for i in 0..<indices.count / 3 {
                try body([
                        vertices[Int(indices[3 * i + 0])],
                        vertices[Int(indices[3 * i + 1])],
                        vertices[Int(indices[3 * i + 2])]
                ])
            }
        } else {
            for i in 0..<vertices.count / 3 {
                try body([
                        vertices[3 * i + 0],
                        vertices[3 * i + 1],
                        vertices[3 * i + 2]
                ])
            }
        }
    }

    public func eachVertex(_ body: (Vertex) throws -> Void) rethrows {
        for vertex in vertices  {
            try body(vertex)
        }
    }

    public mutating func compile(renderer: Renderer) {
        vertexBuffer = renderer.vertexBuffer.init(mesh: self)
    }

}

public extension MutableMesh {

    /// Calculates all the tangent vectors for the mesh.
    public mutating func calculateTangent() {
        if let indices = self.indices {
            for i in 0..<indices.count / 3 {
                let tangent = computeTangent(
                        vertices[Int(indices[3 * i + 0])],
                        vertices[Int(indices[3 * i + 1])],
                        vertices[Int(indices[3 * i + 2])]
                )
                vertices[Int(indices[3 * i + 0])].tangent = tangent
                vertices[Int(indices[3 * i + 1])].tangent = tangent
                vertices[Int(indices[3 * i + 2])].tangent = tangent
            }
        } else {
            for i in 0..<vertices.count / 3 {
                let tangent = computeTangent(
                        vertices[3 * i + 0],
                        vertices[3 * i + 1],
                        vertices[3 * i + 2]
                )
                vertices[3 * i + 0].tangent = tangent
                vertices[3 * i + 1].tangent = tangent
                vertices[3 * i + 2].tangent = tangent
            }
        }
    }

}

public func uvSphere(radius: Double, stacks: Int, slices: Int) -> Mesh {
    var allVertices: [Vertex] = []
    for u in 0..<stacks {
        let theta1 = Double(u + 0) / Double(stacks) * Double.pi
        let theta2 = Double(u + 1) / Double(stacks) * Double.pi

        for v in 0..<slices {
            let phi1 = Double(v + 0) / Double(slices) * 2 * Double.pi
            let phi2 = Double(v + 1) / Double(slices) * 2 * Double.pi

            var vertices = [
                Vertex(position: Vector3<Float>(
                        x: Float(radius * sin(theta1) * cos(phi1)), // r Sin[\[Theta]] Cos[\[Phi]]
                        y: Float(radius * sin(theta1) * sin(phi1)), // r Sin[\[Theta]] Sin[\[Phi]]
                        z: Float(radius * cos(theta1))              // r Cos[\[Theta]]
                ), texCoords: Vector2<Float>(
                        x: Float(u + 0) / Float(stacks),
                        y: Float(v + 0) / Float(slices)
                ).flipped),
                Vertex(position: Vector3<Float>(
                        x: Float(radius * sin(theta1) * cos(phi2)),
                        y: Float(radius * sin(theta1) * sin(phi2)),
                        z: Float(radius * cos(theta1))
                ), texCoords: Vector2<Float>(
                        x: Float(u + 0) / Float(stacks),
                        y: Float(v + 1) / Float(slices)
                ).flipped),
                Vertex(position: Vector3<Float>(
                        x: Float(radius * sin(theta2) * cos(phi2)),
                        y: Float(radius * sin(theta2) * sin(phi2)),
                        z: Float(radius * cos(theta2))
                ), texCoords: Vector2<Float>(
                        x: Float(u + 1) / Float(stacks),
                        y: Float(v + 1) / Float(slices)
                ).flipped),
                Vertex(position: Vector3<Float>(
                        x: Float(radius * sin(theta2) * cos(phi1)),
                        y: Float(radius * sin(theta2) * sin(phi1)),
                        z: Float(radius * cos(theta2))
                ), texCoords: Vector2<Float>(
                        x: Float(u + 1) / Float(stacks),
                        y: Float(v + 0) / Float(slices)
                ).flipped)
            ]

            vertices = vertices.map {
                var vertex = $0
                vertex.normal = normalize(-vertex.position)

                return vertex
            }

            let vertex1 = vertices[0]
            let vertex2 = vertices[1]
            let vertex3 = vertices[2]
            let vertex4 = vertices[3]

            if u == 0 {
                allVertices += [vertex1, vertex3, vertex4]
            } else if (u + 1) == stacks {
                allVertices += [vertex3, vertex1, vertex2]
            } else {
                allVertices += [
                    vertex1, vertex2, vertex4,
                    vertex2, vertex3, vertex4
                ]
            }
        }
    }

    var mesh = SimpleMesh(vertices: allVertices)
    mesh.calculateTangent()
    return mesh
}

fileprivate func computeTangent(_ vertex1: Vertex, _ vertex2: Vertex, _ vertex3: Vertex) -> Vector3<Float> {
    // compute the tangent and bitangent vectors
    let pos1 = vertex1.position
    let pos2 = vertex2.position
    let pos3 = vertex3.position

    let edge1 = pos2 - pos1
    let edge2 = pos3 - pos1

    let uv0 = vertex1.texCoords
    let uv1 = vertex2.texCoords
    let uv2 = vertex3.texCoords

    let deltaUV1 = uv1 - uv0
    let deltaUV2 = uv2 - uv0

    let f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y)

    return normalize(Vector3<Float>(
            x: f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            y: f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            z: f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
    ))
}

fileprivate extension Vector2 {

    var flipped: Vector2<Float> {
        return Vector2<Float>(y as! Float, x as! Float)
    }

}