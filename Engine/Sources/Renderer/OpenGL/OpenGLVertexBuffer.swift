//
//  OpenGLVertexBuffer.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import SGLOpenGL
import SGLMath

/// A type that identifies a OpenGL Vertex Buffer object
public typealias OpenGLVertexBufferID = GLuint

/// A type that identifies a OpenGL Vertex Attribute Array object
public typealias OpenGLVertexAttributeArrayID = GLuint

/// A type that identifies a OpenGL Vertex Element Array object
public typealias OpenGLVertexElementArrayID = GLuint

/// A type that represents a OpenGL vertex count
public typealias OpenGLVertexCount = GLsizei

public class OpenGLVertexBuffer: VertexBuffer {

    /// The OpenGL vertex buffer object ID
    public let vertexBufferObject: OpenGLVertexBufferID

    /// The OpenGL vertex attribute array object ID
    public let vertexAttributeArrayID: OpenGLVertexAttributeArrayID

    /// The OpenGL vertex element array object ID
    public let vertexElementArrayID: OpenGLVertexElementArrayID?

    /// The OpenGL vertex buffer vertex count
    public let vertexCount: OpenGLVertexCount

    /// The type of primitives used for rendering
    private let primitiveType: PrimitiveType

    public convenience required init(mesh: Mesh) {
        self.init(mesh: mesh, primitiveType: .triangle)
    }

    /// Creates a new Vertex Buffer from a mesh object
    public required init(mesh: Mesh, primitiveType: PrimitiveType) {
        self.primitiveType = primitiveType
        self.vertexAttributeArrayID = OpenGLVertexBuffer.allocateVAO()

        glBindVertexArray(vertexAttributeArrayID)
        if let meshIndices = mesh.indices {
            vertexElementArrayID = OpenGLVertexBuffer.allocateEBO()
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexElementArrayID!)

            let indices = meshIndices.map {
                $0.glValue
            }
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.count * MemoryLayout<GLuint>.stride, indices, GL_STATIC_DRAW)
            vertexCount = indices.count.glSize
        } else {
            vertexElementArrayID = nil
            vertexCount = mesh.vertices.count.glSize
        }

        // build VBO
        vertexBufferObject = OpenGLVertexBuffer.allocateVBO()
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject)
        mesh.vertices.withUnsafeBytes { (ptr) in
            glBufferData(GL_ARRAY_BUFFER, mesh.vertices.count * MemoryLayout<Vertex>.stride, ptr.baseAddress, GL_STATIC_DRAW)
        }

        // build VAO
        vertexAttributes = mesh.vertexAttributes
        updateVertexAttributes()
    }

    deinit {
        var vertexBufferObject = self.vertexBufferObject
        glDeleteBuffers(1, &vertexBufferObject)

        if var vertexElementArrayID = self.vertexElementArrayID {
            glDeleteBuffers(1, &vertexElementArrayID)
        }

        var vertexAttributeArrayID = self.vertexAttributeArrayID
        glDeleteVertexArrays(1, &vertexAttributeArrayID)
    }

    private static func allocateVAO() -> OpenGLVertexAttributeArrayID {
        var id: OpenGLVertexAttributeArrayID = 0
        glGenVertexArrays(1, &id)
        return id
    }

    private static func allocateEBO() -> OpenGLVertexElementArrayID {
        var id: OpenGLVertexElementArrayID = 0
        glGenBuffers(1, &id)
        return id
    }

    private static func allocateVBO() -> OpenGLVertexBufferID {
        var id: OpenGLVertexBufferID = 0
        glGenBuffers(1, &id)
        return id
    }

    public func draw() {
        glBindVertexArray(vertexAttributeArrayID)

        if let vertexElementArrayID = self.vertexElementArrayID {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexElementArrayID)
            glDrawElements(primitiveType.glDrawMode, vertexCount, UInt32.glType, nil)
        } else {
            glDrawArrays(primitiveType.glDrawMode, 0, vertexCount)
        }

        glBindVertexArray(0)
    }

    /// The Vertex Buffer vertex attributes structure. The `vertexAttributes`
    /// hold information about how all attributes (position, normals, tex
    /// coords, etc.) are to be fetched from the mesh memory buffer.
    ///
    /// When constructing a new `VertexBuffer`, the renderer-specific
    /// implementation will use data from the source mesh to infer a initial
    /// vertex attribute structure. If the default is not ideal or correct,
    /// a user can change the vertex attributes at any time.
    public var vertexAttributes: [VertexAttribute] {
        didSet {
            updateVertexAttributes()
        }
    }

    private func updateVertexAttributes() {
        glBindVertexArray(vertexAttributeArrayID)
        for attribute in vertexAttributes {
            switch attribute {
            case let .position(params):             attribute.map(index: 0, params: params)
            case let .normal(params):               attribute.map(index: 1, params: params)
            case let .textureCoordinates(params):   attribute.map(index: 2, params: params)
            case let .tangent(params):              attribute.map(index: 3, params: params)
            default: fatalError("Unsupported VertexAttribute: \(attribute)")
            }
        }
        glBindVertexArray(0)
    }

}

fileprivate extension PrimitiveType {
    var glDrawMode: GLenum {
        switch self {
        case .point:            return GL_POINTS
        case .lineStrip:        return GL_LINE_STRIP
        case .lineLoop:         return GL_LINE_LOOP
        case .line:             return GL_LINES
        case .triangleStrip:    return GL_TRIANGLE_STRIP
        case .triangleFan:      return GL_TRIANGLE_FAN
        case .triangle:         return GL_TRIANGLES
        case .quadStrip:        return GL_QUAD_STRIP
        case .quad:             return GL_QUADS
        case .polygon:          return GL_POLYGON
        case .patch:            return GL_PATCHES
        }
    }
}

fileprivate extension VertexAttribute {
    fileprivate func map(index: UInt32, params: VertexAttribute.Params) {
        glEnableVertexAttribArray(index.glValue)
        glVertexAttribPointer(
                index: index.glValue,
                size: params.size.glSize,
                type: Float.glType,
                normalized: params.normalize,
                stride: params.stride.glSize,
                pointer: UnsafeRawPointer(bitPattern: params.offset)
        )
    }
}