//
//  OpenGLRenderer.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/1/17.
//  Copyright © 2017 Rogiel Sulzbach. All rights reserved.
//

import SGLOpenGL

public class OpenGLRenderer: Renderer {

    /// The type that represents the renderer-specific vertex shader
    public var vertexShader: VertexShader.Type {
        return OpenGLVertexShader.self
    }

    /// The type that represents the renderer-specific fragment shader
    public var fragmentShader: FragmentShader.Type {
        return OpenGLFragmentShader.self
    }

    /// The type that represents the renderer-specific vertex shader
    public var geometryShader: GeometryShader.Type {
        return OpenGLGeometryShader.self
    }

    /// The type that represents the renderer-specific vertex shader
    public var shaderProgram: ShaderProgram.Type {
        return OpenGLShaderProgram.self
    }

    /// The type that represents the renderer-specific vertex buffer
    public var vertexBuffer: VertexBuffer.Type {
        return OpenGLVertexBuffer.self
    }

    /// The type that represents the renderer-specific one dimensional texture
    public var texture1D: Texture1D.Type {
        return OpenGLTexture1D.self
    }

    /// The type that represents the renderer-specific two dimensional texture
    public var texture2D: Texture2D.Type {
        return OpenGLTexture2D.self
    }

    /// The type that represents the renderer-specific three dimensional texture
    public var texture3D: Texture3D.Type {
        return OpenGLTexture3D.self
    }

    /// The type that represents the renderer-specific texture
    public var textureCube: TextureCube.Type {
        return OpenGLTextureCube.self
    }

    /// The type that represents the renderer-specific framebuffer
    public var framebuffer: Framebuffer.Type {
        return OpenGLFramebuffer.self
    }

    /// The type that represents the renderer-specific render buffer
    public var renderBuffer: RenderBuffer.Type {
        return OpenGLRenderBuffer.self
    }

    /// Returns the default rendering framebuffer
    public var defaultFramebuffer: Framebuffer

    /// Creates a new OpenGL framebuffer. The currently bound framebuffer will
    /// be used as the default framebuffer.
    public init() {
        var fbo: GLint = 0;
        print("init...")
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &fbo);
        print("done")
        defaultFramebuffer = OpenGLFramebuffer(framebufferID: GLuint(fbo))
    }

}

internal protocol OpenGLNumeric: Numeric {
    static var glType: GLint { get }
}

internal protocol OpenGLFixedWidthInteger: OpenGLNumeric, FixedWidthInteger {
}

internal protocol OpenGLBinaryFloatingPoint: OpenGLNumeric, BinaryFloatingPoint {
}

extension Float: OpenGLBinaryFloatingPoint {
    static let glType: GLint = GL_FLOAT
    var glValue: GLfloat {
        get { return GLfloat(self) }
        set { self = Float(newValue) }
    }
}

extension Double: OpenGLBinaryFloatingPoint {
    static let glType: GLint = GL_DOUBLE
    var glValue: GLdouble {
        get { return GLdouble(self) }
        set { self = Double(newValue) }
    }
}

extension Int8: OpenGLFixedWidthInteger {
    static let glType: GLint = GL_BYTE
    var glValue: GLbyte {
        get { return GLbyte(self) }
        set { self = Int8(newValue) }
    }
}

extension UInt8: OpenGLFixedWidthInteger {
    static let glType: GLint = GL_UNSIGNED_BYTE
    var glValue: GLubyte {
        get { return GLubyte(self) }
        set { self = UInt8(newValue) }
    }
}

extension Int16: OpenGLFixedWidthInteger {
    static let glType: GLint = GL_SHORT
    var glValue: GLshort {
        get { return GLshort(self) }
        set { self = Int16(newValue) }
    }
}

extension UInt16: OpenGLFixedWidthInteger {
    static let glType: GLint = GL_UNSIGNED_SHORT
    var glValue: GLushort {
        get { return GLushort(self) }
        set { self = UInt16(newValue) }
    }
}

extension Int32: OpenGLFixedWidthInteger {
    static let glType: GLint = GL_INT
    var glValue: GLint {
        get { return GLint(self) }
        set { self = Int32(newValue) }
    }
}

extension UInt32: OpenGLFixedWidthInteger {
    static let glType: GLint = GL_UNSIGNED_INT
    var glValue: GLuint {
        get { return GLuint(self) }
        set { self = UInt32(newValue) }
    }
}

extension Bool {
    var glValue: GLboolean {
        get { return GLboolean(self) }
        set { self = Bool(newValue) }
    }
}

extension Int64 {
    var glValue: GLint64 {
        get { return GLint64(self) }
        set { self = Int64(newValue) }
    }
}

extension UInt64 {
    var glValue: GLuint64 {
        get { return GLuint64(self) }
        set { self = UInt64(newValue) }
    }
}

extension Int {
    var glSize: GLsizei {
        get { return GLsizei(self) }
        set { self = Int(newValue) }
    }
    var glInt: GLint {
        get { return GLint(self) }
        set { self = Int(newValue) }
    }
}

extension UInt {
    var glSize: GLsizei {
        get { return GLsizei(self) }
        set { self = UInt(newValue) }
    }
    var glUInt: GLuint {
        get { return GLuint(self) }
        set { self = UInt(newValue) }
    }
}
