//
//  Renderer.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/1/17.
//  Copyright © 2017 Rogiel Sulzbach. All rights reserved.
//

public protocol Renderer : class {
    
    /// The type that represents the renderer-specific vertex shader
    var vertexShader: VertexShader.Type { get }
    
    /// The type that represents the renderer-specific fragment shader
    var fragmentShader: FragmentShader.Type { get }
    
    /// The type that represents the renderer-specific vertex shader
    var geometryShader: GeometryShader.Type { get }
    
    /// The type that represents the renderer-specific vertex shader
    var shaderProgram: ShaderProgram.Type { get }
    
    /// The type that represents the renderer-specific vertex buffer
    var vertexBuffer: VertexBuffer.Type { get }

    /// The type that represents the renderer-specific one dimensional texture
    var texture1D: Texture1D.Type { get }

    /// The type that represents the renderer-specific two dimensional texture
    var texture2D: Texture2D.Type { get }

    /// The type that represents the renderer-specific three dimensional texture
    var texture3D: Texture3D.Type { get }

    /// The type that represents the renderer-specific texture
    var textureCube: TextureCube.Type { get }
    
    /// The type that represents the renderer-specific framebuffer
    var framebuffer: Framebuffer.Type { get }

    /// The type that represents the renderer-specific render buffer
    var renderBuffer: RenderBuffer.Type { get }
    
    /// Returns the default rendering framebuffer
    var defaultFramebuffer: Framebuffer { get }
    
}

public extension Renderer {

}