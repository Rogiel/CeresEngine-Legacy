//
//  DeferredRenderingTechnique.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import SGLMath

public class ForwardRenderingTechnique : RenderingTechnique {

    /// A background skybox
    public var backgroundSkybox: TextureCube? = nil

    internal var defaultFramebuffer: Framebuffer
    internal let lightingShader: ShaderProgram
    internal let skyboxShader: ShaderProgram

    /// A vertex buffer that renders a quad on the entire screen.
    private let cubeVertexBuffer: VertexBuffer

    public init(renderer: Renderer, defaultFramebuffer: Framebuffer) {
        self.defaultFramebuffer = defaultFramebuffer
        self.lightingShader = renderer.shaderProgram.factory.forwardLightingShader!
        self.skyboxShader = renderer.shaderProgram.factory.skyboxShader!

        let vertex2: [[Float]] = [
            [-1.0, 1.0, -1.0],
            [-1.0, -1.0, -1.0],
            [1.0, -1.0, -1.0],
            [1.0, -1.0, -1.0],
            [1.0, 1.0, -1.0],
            [-1.0, 1.0, -1.0],

            [-1.0, -1.0, 1.0],
            [-1.0, -1.0, -1.0],
            [-1.0, 1.0, -1.0],
            [-1.0, 1.0, -1.0],
            [-1.0, 1.0, 1.0],
            [-1.0, -1.0, 1.0],

            [1.0, -1.0, -1.0],
            [1.0, -1.0, 1.0],
            [1.0, 1.0, 1.0],
            [1.0, 1.0, 1.0],
            [1.0, 1.0, -1.0],
            [1.0, -1.0, -1.0],

            [-1.0, -1.0, 1.0],
            [-1.0, 1.0, 1.0],
            [1.0, 1.0, 1.0],
            [1.0, 1.0, 1.0],
            [1.0, -1.0, 1.0],
            [-1.0, -1.0, 1.0],

            [-1.0, 1.0, -1.0],
            [1.0, 1.0, -1.0],
            [1.0, 1.0, 1.0],
            [1.0, 1.0, 1.0],
            [-1.0, 1.0, 1.0],
            [-1.0, 1.0, -1.0],

            [-1.0, -1.0, -1.0],
            [-1.0, -1.0, 1.0],
            [1.0, -1.0, -1.0],
            [1.0, -1.0, -1.0],
            [-1.0, -1.0, 1.0],
            [1.0, -1.0, 1.0]
        ]
        cubeVertexBuffer = renderer.vertexBuffer.init(mesh: SimpleMesh(vertices: vertex2.map {
            return Vertex(position: Vector3<Float>($0[0], $0[1], $0[2]))
        }, indices: nil))
    }

    public func render(scene: Scene) {
        defaultFramebuffer.blending = false
        defaultFramebuffer.depthTest = true

        defaultFramebuffer.active {
            defaultFramebuffer.clear(color: Color<Float>(red: 0.0, green: 0.0, blue: 0.0, alpha: 1.0))
        }
        renderSkybox(scene: scene)

        defaultFramebuffer.active {
            lightingShader.active {
                let projection = scene.camera.projection
                let view = scene.camera.view

                lightingShader["projection"] = Matrix4x4<Float>(projection)
                lightingShader["view"] = Matrix4x4<Float>(view)
                lightingShader["camera.position"] = Vector3<Float>(scene.camera.position)
                lightingShader["near"] = Float(scene.camera.nearPlane)
                lightingShader["far"] = Float(scene.camera.farPlane)

                GameEngine.shared.entityManager.forEach { (renderable: Renderable) in
                    guard let material = renderable.model.material as? PhongMaterial else {
                        return
                    }

                    var textureSlot: Int = 0
                    if let texture = material.diffuseTexture {
                        texture.activate(slot: textureSlot)
                        lightingShader["material.diffuse"] = textureSlot
                        textureSlot += 1
                    }
                    if let texture = material.specularMap {
                        texture.activate(slot: textureSlot)
                        lightingShader["material.specular"] = textureSlot
                        textureSlot += 1
                    }
                    if let texture = material.normalMap {
                        texture.activate(slot: textureSlot)
                        lightingShader["material.normal"] = textureSlot
                        textureSlot += 1
                    }

                    let model = renderable.transform
                    lightingShader["model"] = Matrix4x4<Float>(model)
                    lightingShader["MVP"] = Matrix4x4<Float>(projection * view * model)
                    if let vertexBuffer = renderable.model.mesh.vertexBuffer {
                        vertexBuffer.draw()
                    }
                }
            }
        }
    }

    /// Renders the Skybox if a skybox texture is set
    private func renderSkybox(scene: Scene) {
        if let backgroundSkybox = backgroundSkybox {
            defaultFramebuffer.depthTestingMode = .lessThanOrEqual

            defaultFramebuffer.active {
                skyboxShader.active {
                    skyboxShader["skybox"] = 0
                    backgroundSkybox.activate(slot: 0)

                    skyboxShader["gamma"] = Float(1.0)
                    skyboxShader["projection"] = Matrix4x4<Float>(scene.camera.projection)
                    skyboxShader["view"] = Matrix4x4<Float>(scene.camera.view)

                    cubeVertexBuffer.draw()
                }
            }

            defaultFramebuffer.depthTestingMode = .less
        }
    }

}


public protocol ForwardRenderingShaderProgramFactory : ShaderProgramFactory {

    /// The Forward Lighting shader program
    var forwardLightingShader: ShaderProgram? { get }

}

fileprivate extension ShaderProgramFactory {

    var forwardLightingShader: ShaderProgram? {
        return self.for(ForwardRenderingShaderProgramFactory.self)?.forwardLightingShader
    }

}