//
//  OpenGLShaderProgram.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/2/17.
//

import Foundation
import SGLOpenGL

/// A type that identifies a OpenGL Shader Program object
public typealias OpenGLShaderProgramID = GLuint

public class OpenGLShaderProgram : ShaderProgram {

    /// The OpenGL shader program object ID
    public let programID: OpenGLShaderProgramID;
    
    /// A cache of all known uniforms locations
    private var uniformLocations:[String: GLint] = [:]
    
    public required init(vertexShader: VertexShader, fragmentShader: FragmentShader, geometryShader: GeometryShader?) throws {
        guard let vertexShader = vertexShader as? OpenGLVertexShader else {
            throw ShaderCompilationError.invalidVertexShaderType
        }
        
        guard let fragmentShader = fragmentShader as? OpenGLFragmentShader else {
            throw ShaderCompilationError.invalidFragmentShaderType
        }
        
        let geometryShader = geometryShader as? OpenGLGeometryShader
     
        programID = glCreateProgram()
        glProgramParameteri(programID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE)
        if let errorMessage = linkProgram(programID, vertex: vertexShader.shaderID, fragment: fragmentShader.shaderID, geometry: geometryShader?.shaderID) {
            glDeleteProgram(self.programID)
            throw ShaderCompilationError.other(error: errorMessage)
        }
    }
    
    
    /// Creates a new shader program from a `vertexShader`, a `fragmentShader`.
    public convenience required init(vertexShader: VertexShader, fragmentShader: FragmentShader) throws {
        try self.init(vertexShader: vertexShader, fragmentShader: fragmentShader, geometryShader: nil)
    }
    
    public required convenience init(vertexShaderSource: String, fragmentShaderSource: String, geometryShaderSource: String?) throws {
        if let geometryShaderSource = geometryShaderSource {
            try self.init(vertexShader: OpenGLVertexShader(compilingSource: vertexShaderSource),
                          fragmentShader: OpenGLFragmentShader(compilingSource: fragmentShaderSource),
                          geometryShader: OpenGLGeometryShader(compilingSource: geometryShaderSource))
        } else {
            try self.init(vertexShader: OpenGLVertexShader(compilingSource: vertexShaderSource),
                          fragmentShader: OpenGLFragmentShader(compilingSource: fragmentShaderSource))
        }
    }
    
    /// Creates a new shader program from a `vertexShaderSource`, a `fragmentShaderSource`.
    public convenience required init(vertexShaderSource: String, fragmentShaderSource: String) throws {
        try self.init(vertexShaderSource: vertexShaderSource, fragmentShaderSource: fragmentShaderSource, geometryShaderSource: nil)
    }
    
    deinit {
        glDeleteProgram(programID)
    }

    /// Sets a shader uniform variable.
    ///
    /// Implementations are recommended to cache uniforms if getting their handle
    /// is a expensive operation.
    ///
    /// - Parameter name: the uniform name
    /// - Parameter value: the uniform value
    public func setUniform<T>(_ name: String, value: T) where T : Uniformable {
        setUniform(name, value: value as Uniformable)
    }

    /// Sets a shader uniform variable.
    ///
    /// Implementations are recommended to cache uniforms if getting their handle
    /// is a expensive operation.
    ///
    /// - Parameter name: the uniform name
    /// - Parameter value: the uniform value
    public func setUniform(_ name: String, value: Uniformable) {
        var uniformLocation: GLint = 0
        if uniformLocations.index(forKey: name) == nil {
            uniformLocation = glGetUniformLocation(programID, name)
            uniformLocations[name] = uniformLocation
        } else {
            uniformLocation = uniformLocations[name]!
        }
        
        active {
            if let value = value as? ScalarUniformable {
                switch value.scalarUniformValue {
                case let .integer(n):   glUniform1i(uniformLocation, GLint(n))
                case let .float(f):     glUniform1f(uniformLocation, f)
                case let .double(d):    glUniform1d(uniformLocation, d)
                case let .boolean(b):   glUniform1i(uniformLocation, b ? 1 : 0)
                }
            } else if let value = value as? VectorUniformable {
                switch value.vectorUniformValue {
                case let .integer(value, count):
                    switch count {
                    case 2: glUniform2i(uniformLocation, GLint(value[0]), GLint(value[1]))
                    case 3: glUniform3i(uniformLocation, GLint(value[0]), GLint(value[1]), GLint(value[2]))
                    case 4: glUniform4i(uniformLocation, GLint(value[0]), GLint(value[1]), GLint(value[2]), GLint(value[3]))
                    default: fatalError("Vectors larger than 4 are not supported by OpenGL")
                    }
                    
                case let .float(value, count):
                    switch count {
                    case 2: glUniform2f(uniformLocation, GLfloat(value[0]), GLfloat(value[1]))
                    case 3: glUniform3f(uniformLocation, GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]))
                    case 4: glUniform4f(uniformLocation, GLfloat(value[0]), GLfloat(value[1]), GLfloat(value[2]), GLfloat(value[3]))
                    default: fatalError("Vectors larger than 4 are not supported by OpenGL")
                    }
                    
                case let .double(value, count):
                    switch count {
                    case 2: glUniform2d(uniformLocation, GLdouble(value[0]), GLdouble(value[1]))
                    case 3: glUniform3d(uniformLocation, GLdouble(value[0]), GLdouble(value[1]), GLdouble(value[2]))
                    case 4: glUniform4d(uniformLocation, GLdouble(value[0]), GLdouble(value[1]), GLdouble(value[2]), GLdouble(value[3]))
                    default: fatalError("Vectors larger than 4 are not supported by OpenGL")
                    }
                    
                case let .boolean(value, count):
                    switch count {
                    case 2: glUniform2i(uniformLocation, value[0] ? 1 : 0, value[1] ? 1 : 0)
                    case 3: glUniform3i(uniformLocation, value[0] ? 1 : 0, value[1] ? 1 : 0, value[2] ? 1 : 0)
                    case 4: glUniform4i(uniformLocation, value[0] ? 1 : 0, value[1] ? 1 : 0, value[2] ? 1 : 0, value[3] ? 1 : 0)
                    default: fatalError("Vectors larger than 4 are not supported by OpenGL")
                    }
                }
            } else if let value = value as? MatrixUniformable {
                switch value.matrixUniformValue {
                case .integer(_, _, _):
                    fatalError("Integer matrices are not supported by OpenGL")
                    
                case let .float(value, rows, cols):
                    switch (rows, cols) {
                    case (2, 2): glUniformMatrix2fv  (uniformLocation, 1, false, value)
                    case (2, 3): glUniformMatrix2x3fv(uniformLocation, 1, false, value)
                    case (2, 4): glUniformMatrix2x4fv(uniformLocation, 1, false, value)
                        
                    case (3, 2): glUniformMatrix3x2fv(uniformLocation, 1, false, value)
                    case (3, 3): glUniformMatrix3fv  (uniformLocation, 1, false, value)
                    case (3, 4): glUniformMatrix3x4fv(uniformLocation, 1, false, value)
                        
                    case (4, 2): glUniformMatrix4x2fv(uniformLocation, 1, false, value)
                    case (4, 3): glUniformMatrix4x3fv(uniformLocation, 1, false, value)
                    case (4, 4): glUniformMatrix4fv  (uniformLocation, 1, false, value)
                    default: fatalError("Matrices larger than 4x4 are not supported by OpenGL")
                    }
                    
                case let .double(value, rows, cols):
                    switch (rows, cols) {
                    case (2, 2): glUniformMatrix2dv  (uniformLocation, 1, false, value)
                    case (2, 3): glUniformMatrix2x3dv(uniformLocation, 1, false, value)
                    case (2, 4): glUniformMatrix2x4dv(uniformLocation, 1, false, value)
                        
                    case (3, 2): glUniformMatrix3x2dv(uniformLocation, 1, false, value)
                    case (3, 3): glUniformMatrix3dv  (uniformLocation, 1, false, value)
                    case (3, 4): glUniformMatrix3x4dv(uniformLocation, 1, false, value)
                        
                    case (4, 2): glUniformMatrix4x2dv(uniformLocation, 1, false, value)
                    case (4, 3): glUniformMatrix4x3dv(uniformLocation, 1, false, value)
                    case (4, 4): glUniformMatrix4dv  (uniformLocation, 1, false, value)
                    default: fatalError("Matrices larger than 4x4 are not supported by OpenGL")
                    }
                    
                case .boolean(_, _, _):
                    fatalError("Boolean matrices are not supported by OpenGL")
                }
            } else {
                fatalError("Unknown Uniformable type")
            }
        }
    }
    
    public func activate() {
        glUseProgram(self.programID)
        self.isActive = true

        for slot in GL_TEXTURE0...GL_TEXTURE31 {
            glActiveTexture(slot)
            glBindTexture(GL_TEXTURE_2D, 0)
        }
    }
    
    public func deactivate() {
        glUseProgram(0)
        self.isActive = false
    }
    
    /// Set if the shader is currently active
    public var isActive: Bool = false

    /// The default shaders
    public static var factory: ShaderProgramFactory = OpenGLShaderFactory()

    /// Checks whether the `ShaderProgram` implementation supports exporting a
    /// compiled binary representation of a shader.
    public static var supportsBinaryRepresentation: Bool {
        var n: GLint = 0
        glGetIntegerv(pname: GL_NUM_PROGRAM_BINARY_FORMATS, data: &n)
        return n != 0
    }

    /// Creates a new shader program from its `binaryRepresentation`.
    public required init?(binaryRepresentation data: Data) {
        programID = glCreateProgram()

        let binaryFormat = data.withUnsafeBytes { (pointer: UnsafePointer<GLenum>) in
            return pointer.pointee
        }
        let programBinary = data.advanced(by: MemoryLayout<GLenum>.stride)

        programBinary.withUnsafeBytes { (pointer) in
            glProgramBinary(
                    program: programID,
                    binaryFormat: binaryFormat,
                    binary: pointer,
                    length: GLsizei(programBinary.count))
        }

        if glGetError() != 0 {
            return nil
        }
    }

    /// Returns a binary representation for the shader program. If the backend does not
    /// support binaries representations, nil can be returned.
    public var binaryRepresentation: Data? {
        var length: GLint = 0
        glGetProgramiv(
                program: programID,
                pname: GL_PROGRAM_BINARY_LENGTH,
                params: &length)
        if length == 0 {
            return nil
        }

        var data = Data(count: Int(length) + MemoryLayout<GLenum>.stride)
        data.withUnsafeMutableBytes { (pointer) in
            glGetProgramBinary(
                    program: programID,
                    bufSize: length,
                    length: nil,
                    binaryFormat: pointer,
                    binary: pointer.advanced(by: MemoryLayout<GLenum>.stride))
        }

        if glGetError() != 0 {
            return nil
        }

        return data
    }

}

/// A type that identifies a OpenGL Shader object
public typealias OpenGLShaderID = GLuint

public class OpenGLVertexShader : VertexShader {

    /// The OpenGL shader object ID
    public let shaderID: OpenGLShaderID
    
    public required init(compilingSource source: String) throws {
        shaderID = glCreateShader(type: GL_VERTEX_SHADER)
        if let errorMessage = compileShader(shaderID, source: source) {
            glDeleteShader(shaderID)
            throw ShaderCompilationError.invalidVertexShaderSource(error: errorMessage)
        }
    }

    deinit {
        glDeleteShader(shaderID)
    }
    
}

public class OpenGLFragmentShader : FragmentShader {
    
    /// The shader ID
    public let shaderID: OpenGLShaderID
    
    public required init(compilingSource source: String) throws {
        shaderID = glCreateShader(type: GL_FRAGMENT_SHADER)
        if let errorMessage = compileShader(shaderID, source: source) {
            glDeleteShader(shaderID)
            throw ShaderCompilationError.invalidFragmentShaderSource(error: errorMessage)
        }
    }
    
    deinit {
        glDeleteShader(shaderID)
    }
    
}

public class OpenGLGeometryShader : GeometryShader {
    
    /// The shader ID
    public let shaderID: OpenGLShaderID
    
    public required init(compilingSource source: String) throws {
        shaderID = glCreateShader(type: GL_GEOMETRY_SHADER)
        if let errorMessage = compileShader(shaderID, source: source) {
            glDeleteShader(shaderID)
            throw ShaderCompilationError.invalidGeometryShaderSource(error: errorMessage)
        }
    }
    
    deinit {
        glDeleteShader(shaderID)
    }
    
}

/// Compiles a shader.
///
/// - Parameter shader: the shader ID to be compile to
/// - Parameter source: the shader source to be compiled
fileprivate func compileShader(_ shader: OpenGLShaderProgramID, source: String) -> String? {
    source.withCString {
        var s = [$0]
        glShaderSource(shader: shader, count: 1, string: &s, length: nil)
    }
    glCompileShader(shader)
    
    var success:GLint = 0
    glGetShaderiv(shader: shader, pname: GL_COMPILE_STATUS, params: &success)
    
    if success != GL_TRUE {
        var logSize:GLint = 0
        glGetShaderiv(shader: shader, pname: GL_INFO_LOG_LENGTH, params: &logSize)
        if logSize == 0 { return "" }
        var infoLog = [GLchar](repeating: 0, count: Int(logSize))
        glGetShaderInfoLog(shader: shader, bufSize: logSize, length: nil, infoLog: &infoLog)
        return String(cString:infoLog)
    }
    
    return nil
    
}

fileprivate func linkProgram(_ program: OpenGLShaderProgramID, vertex: OpenGLShaderID, fragment: OpenGLShaderID, geometry: OpenGLShaderID?) -> String? {
    glAttachShader(program, vertex)
    glAttachShader(program, fragment)
    if let geometry = geometry {
        glAttachShader(program, geometry)
    }
    
    glLinkProgram(program)
    
    var success:GLint = 0
    glGetProgramiv(program: program, pname: GL_LINK_STATUS, params: &success)
    
    if success != GL_TRUE {
        var logSize:GLint = 0
        glGetProgramiv(program: program, pname: GL_INFO_LOG_LENGTH, params: &logSize)
        if logSize == 0 { return "" }
        var infoLog = [GLchar](repeating: 0, count: Int(logSize))
        glGetProgramInfoLog(program: program, bufSize: logSize, length: nil, infoLog: &infoLog)
        
        return String(cString:infoLog)
    }
    
    return nil
}

public class OpenGLShaderFactory : ShaderProgramFactory,
        ForwardRenderingShaderProgramFactory, DeferredRenderingShaderProgramFactory,
        PBRDeferredRenderingShaderProgramFactory{

    /// The BloomBlur shader program
    public lazy var bloomBlurShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.bloomBlurVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.bloomBlurFragmentShaderSource
    )

    /// The BloomExposureMapping shader program
    public lazy var bloomExposureMappingShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.bloomExposureMappingVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.bloomExposureMappingFragmentShaderSource
    )

    /// The Skybox shader program
    public lazy var skyboxShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.skyboxVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.skyboxFragmentShaderSource
    )

    /// The HDR shader program
    public lazy var hdrShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.hdrVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.hdrFragmentShaderSource
    )

    /// The DeferredDirectionalLightShadowMap shader program
    public lazy var directionalShadowMapShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.directionalShadowMapVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.directionalShadowMapFragmentShaderSource
    )

    /// The DeferredPointLightShadowMap shader program
    public lazy var cubicShadowMapShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.cubicShadowMapVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.cubicShadowMapFragmentShaderSource
    )

    /// The ForwardLighting shader program
    public lazy var forwardLightingShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.forwardLightingVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.forwardLightingFragmentShaderSource
    )

    /// The DeferredPointLight shader program
    public lazy var deferredPointLightShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.deferredPointLightVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.deferredPointLightFragmentShaderSource
    )

    /// The DeferredSpotLight shader program
    public lazy var deferredSpotLightShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.deferredSpotLightVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.deferredSpotLightFragmentShaderSource
    )

    /// The DeferredGeometry shader program
    public lazy var deferredGeometryShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.deferredGeometryVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.deferredGeometryFragmentShaderSource
    )

    /// The DeferredDirectionalLight shader program
    public lazy var deferredDirectionalLightShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.deferredDirectionalLightVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.deferredDirectionalLightFragmentShaderSource
    )

    /// A debug shader program that prints a texture
    public lazy var debugTextureShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.debugTextureVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.debugTextureFragmentShaderSource
    )

    // MARK: - PBR

    /// The PBRDeferredGeometry shader program
    public lazy var pbrDeferredGeometryShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.pbrDeferredGeometryVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.pbrDeferredGeometryFragmentShaderSource
    )

    /// The PBRDeferredPointLight shader program
    public lazy var pbrDeferredPointLightShader: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.pbrDeferredLightingVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.pbrDeferredLightingFragmentShaderSource
    )

    /// The PBRDeferredSpotLight shader program
    public lazy var pbrDeferredSpotLightShader: ShaderProgram? = try! OpenGLShaderProgram(
            // FIXME
            vertexShaderSource:     OpenGLShaderProgram.pbrDeferredLightingVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.pbrDeferredLightingFragmentShaderSource
    )

    /// The PBRDeferredDirectionalLight shader program
    public lazy var pbrDeferredDirectionalLightShader: ShaderProgram? = try! OpenGLShaderProgram(
            // FIXME
            vertexShaderSource:     OpenGLShaderProgram.pbrDeferredLightingVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.pbrDeferredLightingFragmentShaderSource
    )

    /// The BRDF function for PBR renders
    public lazy var pbrBRDF: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.pbrbrdfVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.pbrbrdfFragmentShaderSource
    )

    /// A shader that performs the irradiance convolution
    public lazy var pbrIrradianceConvolution: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.pbrIrradianceConvolutionVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.pbrIrradianceConvolutionFragmentShaderSource
    )

    /// A shader that performs the PBR prefilter
    public lazy var pbrPrefilter: ShaderProgram? = try! OpenGLShaderProgram(
            vertexShaderSource:     OpenGLShaderProgram.pbrPrefilterVertexShaderSource,
            fragmentShaderSource:   OpenGLShaderProgram.pbrPrefilterFragmentShaderSource
    )

}