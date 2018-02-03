//
//  VertexBuffer.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

public protocol VertexBuffer : class {

    /// Creates a new Vertex Buffer from a mesh object.
    /// Defaults the primitive type to .triangles.
    init(mesh: Mesh)

    /// Creates a new Vertex Buffer from a mesh object
    init(mesh: Mesh, primitiveType: PrimitiveType)

    /// Draws the vertex buffer object
    func draw()

    /// The Vertex Buffer vertex attributes structure. The `vertexAttributes`
    /// hold information about how all attributes (position, normals, tex
    /// coords, etc.) are to be fetched from the mesh memory buffer.
    ///
    /// When constructing a new `VertexBuffer`, the renderer-specific
    /// implementation will use data from the source mesh to infer a initial
    /// vertex attribute structure. If the default is not ideal or correct,
    /// a user can change the vertex attributes at any time.
    var vertexAttributes: [VertexAttribute] { get set }

}

/// A vertex attribute holds information about how the rendering engine should
/// map the mesh memory data into individual vertex input in shaders.
///
/// Each rendering engine have a different limit on the maximum amount of vertex
/// attributes supported per `VertexBuffer` object.
public enum VertexAttribute {
    /// A vertex attribute that maps the vertex position vector
    case position(Params)

    /// A vertex attribute that maps vertex normal vector
    case normal(Params)

    /// A vertex attribute that maps the vertex texture coordinates
    case textureCoordinates(Params)

    /// A vertex attribute that maps the vertex texture tangent vector
    case tangent(Params)

    /// A vertex attribute that maps a custom vertex attribute
    ///
    /// A custom vertex attribute must have a name and a regular `Params`
    /// structure. The name is only used internally for debug.
    case custom(name: String, Params)

    /// A structure with attribute parameters
    public struct Params {
        /// The amount of elements (in the vertex vector) in the attribute.
        ///
        /// If mapping a Vector3, the `size` must be `3`. If mapping a 4x4
        /// matrix, the `size` must be `16`.
        public let size: Int

        /// The attribute type that the rendering engine should use when
        /// interpreting the values in the buffer.
        ///
        /// Currently, only Float is supported.
        public let type: Float.Type

        /// A flag indicating if the rendering engine should normalize the value
        /// before using it in the shader.
        ///
        /// By default, the parameter is set not be normalized by the rendering
        /// engine to reduce the amount of computation needed.
        public let normalize: Bool

        /// The stride (the spacing between two values vertices values)
        ///
        /// In most cases, this is the size (or stride) of the `Vertex`
        /// structure.
        public let stride: Int

        /// The attribute offset inside the vertex.
        ///
        /// In most cases, this is the sum of all sizes (or strides) of all
        /// attributes preceding this in the `Vertex` attribute.
        public let offset: Int

        /// Creates a new VertexAttribute parameters structure
        public init(size: Int, type: Float.Type = Float.self,
                    normalize: Bool = false, stride: Int, offset: Int) {
            self.size = size
            self.type = type
            self.normalize = normalize
            self.stride = stride
            self.offset = offset
        }
    }
}