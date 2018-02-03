//
//  Shader.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/1/17.
//  Copyright © 2017 Rogiel Sulzbach. All rights reserved.
//

import Foundation
import SGLMath

public protocol ShaderProgram : class {
    
    /// Creates a new shader program from a `vertexShader`, a `fragmentShader` and a
    /// optional `geometryShader`.
    init(vertexShader: VertexShader, fragmentShader: FragmentShader, geometryShader: GeometryShader?) throws
    
    /// Creates a new shader program from a `vertexShader`, a `fragmentShader`.
    init(vertexShader: VertexShader, fragmentShader: FragmentShader) throws
    
    /// Creates a new shader program from a `vertexShaderSource`, a `fragmentShaderSource`
    /// and a optional `geometryShaderSource`.
    init(vertexShaderSource: String, fragmentShaderSource: String, geometryShaderSource: String?) throws
   
    /// Creates a new shader program from a `vertexShaderSource`, a `fragmentShaderSource`.
    init(vertexShaderSource: String, fragmentShaderSource: String) throws
    
    /// Creates a new shader program from its `binaryRepresentation`.
    init?(binaryRepresentation: Data)
    
    /// Activates a shader program
    func activate()
    
    /// Deactivates a shader program
    func deactivate()
    
    /// Executes a block within the context of the shader program
    func active(body: () throws -> Void) rethrows
    
    // MARK: - Operations
    
    /// Sets a shader uniform variable.
    ///
    /// Implementations are recommended to cache uniforms if getting their handle
    /// is a expensive operation.
    ///
    /// - Parameter name: the uniform name
    /// - Parameter value: the uniform value
    func setUniform<T>(_ name: String, value: T) where T : Uniformable

    /// Sets a shader uniform variable.
    ///
    /// Implementations are recommended to cache uniforms if getting their handle
    /// is a expensive operation.
    ///
    /// - Parameter name: the uniform name
    /// - Parameter value: the uniform value
    func setUniform(_ name: String, value: Uniformable)
    
    // MARK: - Representations
    
    /// Returns a binary representation for the shader program. If the backend does not
    /// support binaries representations, nil can be returned.
    var binaryRepresentation: Data? { get }
    
    // MARK: - State
    
    /// Set if the shader is currently active
    var isActive: Bool { get }

    /// The default shaders
    static var factory: ShaderProgramFactory { get }

}

public extension ShaderProgram {
    
    /// Creates a new shader program from its `binaryRepresentation`.
    public init?(binaryRepresentation: Data) {
        return nil
    }
    
    /// Executes a block within the context of the shader program
    public func active(body: () throws -> Void) rethrows {
        if isActive {
            try body()
        } else {
            activate()
            try body()
            deactivate()
        }
    }

    /// Checks whether the `ShaderProgram` implementation supports exporting a
    /// compiled binary representation of a shader.
    public static var supportsBinaryRepresentation: Bool {
        return false
    }

    /// Returns a binary representation for the shader program. If the backend does not
    /// support binaries representations, nil can be returned.
    public var binaryRepresentation: Data? {
        return nil
    }

    /// Set a dictionary of uniforms
    public func setUniforms(_ uniforms: [String: Uniformable]) {
        for (name, value) in uniforms {
            setUniform(name, value: value)
        }
    }

    /// Set a dictionary of uniforms
    public func setUniforms(prefix: String, _ uniforms: [String: Uniformable]) {
        for (name, value) in uniforms {
            setUniform(prefix + name, value: value)
        }
    }

    /// A subscript helper that sets a uniform value.
    public subscript(_ name: String) -> Uniformable {
        get {
            return Double(0.0)
        }
        set(uniform) {
            self.setUniform(name, value: uniform)
        }
    }
    
}

public protocol ShaderProgramFactory : class {

    /// The Bloom Blur shader program
    var bloomBlurShader: ShaderProgram? { get }

    /// The Bloom Exposure Mapping shader program
    var bloomExposureMappingShader: ShaderProgram? { get }

    /// The Skybox shader program
    var skyboxShader: ShaderProgram? { get }

    /// The HDR shader program
    var hdrShader: ShaderProgram? { get }

    /// The Directional Shadow Map shader program
    var directionalShadowMapShader: ShaderProgram? { get }

    /// The Cubic ShadowMap shader program
    var cubicShadowMapShader: ShaderProgram? { get }

    // MARK: - PBR

    /// The BRDF function for PBR renders
    var pbrBRDF: ShaderProgram? { get }

    /// A shader that performs the irradiance convolution
    var pbrIrradianceConvolution: ShaderProgram? { get }

    /// A shader that performs the PBR prefilter
    var pbrPrefilter: ShaderProgram? { get }

    // MARK: - Debug

    /// A debug shader program that prints a texture
    var debugTextureShader: ShaderProgram? { get }

}

public extension ShaderProgramFactory {

    public func `for`<T>(_ type: T.Type) -> T? {
        if let factory = self as? T {
            return factory
        }
        return nil
    }

}

public protocol Shader : class {
    
    /// Compiles a shader from a source
    init(compilingSource source: String) throws
    
}

public protocol VertexShader : Shader {
    
}

public protocol FragmentShader : Shader {
    
}

public protocol GeometryShader : Shader {
    
}

public protocol Uniformable {
    
}

public protocol ScalarUniformable : Uniformable {
    
    /// Returns the uniform representation of the object.
    var scalarUniformValue: ScalarUniformValue { get }
    
}

public protocol VectorUniformable : Uniformable {
    
    /// Returns the uniform representation of the object.
    var vectorUniformValue: VectorUniformValue { get }
    
}

public protocol MatrixUniformable : Uniformable {
    
    /// Returns the uniform representation of the object.
    var matrixUniformValue: MatrixUniformValue { get }
    
}

public enum ScalarUniformValue {
    /// Binds the uniform value as a integer
    case integer(value: UInt32)
    
    /// Binds the uniform value as a float
    case float(value: Float)
    
    /// Binds the uniform value as a double
    case double(value: Double)
    
    /// Binds the uniform value as a boolean
    case boolean(value: Bool)
}

public enum VectorUniformValue {
    /// Binds the uniform value as a integer vector
    case integer(value: [UInt32], count: Int)
    
    /// Binds the uniform value as a float vector
    case float(value: [Float], count: Int)
    
    /// Binds the uniform value as a double vector
    case double(value: [Double], count: Int)
    
    /// Binds the uniform value as a boolean vector
    case boolean(value: [Bool], count: Int)
}

public enum MatrixUniformValue {
    /// Binds the uniform value as a integer matrix
    case integer(value: [UInt32], rows: Int, columns: Int)
    
    /// Binds the uniform value as a float matrix
    case float(value: [Float], rows: Int, columns: Int)
    
    /// Binds the uniform value as a double matrix
    case double(value: [Double], rows: Int, columns: Int)
    
    /// Binds the uniform value as a boolean matrix
    case boolean(value: [Bool], rows: Int, columns: Int)
}

public enum ShaderCompilationError : Error {
    
    case invalidVertexShaderType
    case invalidFragmentShaderType
    case invalidGeometryShaderType
    
    case invalidVertexShaderSource(error: String?)
    case invalidFragmentShaderSource(error: String?)
    case invalidGeometryShaderSource(error: String?)

    case other(error: String)
}

// MARK: - Uniformable extensions

extension Int    : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .integer(value: UInt32(self)) }}
extension UInt   : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .integer(value: UInt32(self)) }}
extension UInt8  : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .integer(value: UInt32(self)) }}
extension UInt16 : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .integer(value: UInt32(self)) }}
extension UInt32 : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .integer(value: UInt32(self)) }}
extension Int8   : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .integer(value: UInt32(self)) }}
extension Int16  : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .integer(value: UInt32(self)) }}
extension Int32  : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .integer(value: UInt32(self)) }}
extension Float  : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .float  (value: self) }}
extension Double : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .double (value: self) }}
extension Bool   : ScalarUniformable { public var scalarUniformValue: ScalarUniformValue { return .boolean(value: self) }}

extension Matrix4x4 : MatrixUniformable {
    public var matrixUniformValue: MatrixUniformValue {
        assert(elements.count == 4 * 4)
        return .float(
            value: elements as! [Float],
            rows: 4, columns: 4
        )
    }
}

extension Vector2 : VectorUniformable {
    public var vectorUniformValue: VectorUniformValue {
        return .float(
            value: elements as! [Float],
            count: 2
        )
    }
}

extension Vector3 : VectorUniformable {
    public var vectorUniformValue: VectorUniformValue {
        return .float(
            value: elements as! [Float],
            count: 3
        )
    }
}

extension Vector4 : VectorUniformable {
    public var vectorUniformValue: VectorUniformValue {
        return .float(
            value: elements as! [Float],
            count: 4
        )
    }
}

extension Color : VectorUniformable {
    public var vectorUniformValue: VectorUniformValue {
        return Vector3<Float>(x: red as! Float, y: green as! Float, z: blue as! Float).vectorUniformValue
    }
}
