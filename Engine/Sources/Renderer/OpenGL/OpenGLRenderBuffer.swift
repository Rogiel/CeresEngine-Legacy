//
// Created by Rogiel Sulzbach on 10/28/17.
//

import SGLOpenGL

/// A type that identifies a OpenGL renderbuffer object
public typealias OpenGLRenderBufferID = GLuint

public class OpenGLRenderBuffer: RenderBuffer {

    /// The OpenGL render buffer object ID
    public let renderBufferID: OpenGLRenderBufferID

    /// Creates a new render buffer with the given parameters
    public required init(width: UInt, height: UInt, format: RenderBufferFormat) {
        self.renderBufferID = OpenGLRenderBuffer.allocate()
        self.format = format
        self.width = width
        self.height = height

        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID)
        glRenderbufferStorage(GL_RENDERBUFFER, format.glInternalformat,
                GLsizei(width), GLsizei(height))
        glBindRenderbuffer(GL_RENDERBUFFER, 0)
    }

    deinit {
        var renderBufferID = self.renderBufferID
        glDeleteBuffers(1, &renderBufferID)
    }

    private static func allocate() -> OpenGLRenderBufferID {
        var id: OpenGLRenderBufferID = 0
        glGenRenderbuffers(1, &id)
        return id
    }

    /// The render buffer format
    public let format: RenderBufferFormat

    /// The render buffer width
    public var width: UInt {
        didSet {
            glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID)
            glRenderbufferStorage(GL_RENDERBUFFER, format.glInternalformat,
                    GLsizei(width), GLsizei(height))
            glBindRenderbuffer(GL_RENDERBUFFER, 0)
        }
    }

    /// The render buffer height
    public var height: UInt {
        didSet {
            glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID)
            glRenderbufferStorage(GL_RENDERBUFFER, format.glInternalformat,
                    GLsizei(width), GLsizei(height))
            glBindRenderbuffer(GL_RENDERBUFFER, 0)
        }
    }

}

fileprivate extension RenderBufferFormat {
    var glInternalformat: GLenum {
        switch self {
        case let .color(precision, 1): switch precision {
        case .uint8:   return GL_R8
        case .float16: return GL_R16F
        case .float32: return GL_R32F
        }

        case let .color(precision, 2): switch precision {
        case .uint8:   return GL_RG8
        case .float16: return GL_RG16F
        case .float32: return GL_RG32F
        }

        case let .color(precision, 3): switch precision {
        case .uint8:   return GL_RGB8
        case .float16: return GL_RGB16F
        case .float32: return GL_RGB32F
        }

        case let .color(precision, 4): switch precision {
        case .uint8:   return GL_RGBA8
        case .float16: return GL_RGBA16F
        case .float32: return GL_RGBA32F
        }

        case .color: fatalError("OpenGL does not supported render targets with more than 4 channels.")

        case let .depth(precision):
            switch precision {
            case .uint8: return GL_DEPTH_COMPONENT16
            case .float16: return GL_DEPTH_COMPONENT16
            case .float32: return GL_DEPTH_COMPONENT32F
            }
        case .stencil: return GL_STENCIL_INDEX8
        }
    }
}