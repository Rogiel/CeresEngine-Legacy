//
//  DeferredRenderingTechnique.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import Foundation
import SGLMath

public class DeferredRenderingTechnique: RenderingTechnique {

    // MARK: - Parameters

    /// Enables or disables HDR colors
    public var hdr: Bool = true

    /// The gamma value to be used when applying color correction
    public var gamma: Float = 2.2

    /// Enables or disables the Bloom effect
    public var bloom: Bool = true

    /// The number of bloom iterations to be performed
    public var bloomAmount: Int = 1

    /// Enables or disables lighting shadows
    public var shadows: Bool = true

    /// A background skybox
    public var backgroundSkybox: TextureCube? = nil

    // MARK: - Framebuffers & Render targets

    /// The deferred renderer output framebuffer.
    /// Most of the times it is desired to write the deferred renderer
    /// output to the default framebuffer.
    private var outputFramebuffer: Framebuffer

    /// The Geometry Buffer
    private var gbuffer: GBuffer

    /// The framebuffer where lighting calculations take place.
    private var renderingFramebuffer: Framebuffer

    /// The texture where the Gbuffer lighting calculations are placed.
    /// This is where shading occurs.
    private var lightingTexture: Texture2D

    private var shadowMapFramebuffer: Framebuffer
    private var shadowMap: Texture2D

    /// A framebuffer that executes the bloom vertical blur
    private var bloomVerticalFramebuffer: Framebuffer

    /// A texture where the bloom vertical blur is renderered to
    private var bloomVerticalTexture: Texture2D

    /// A framebuffer that executes the bloom horizontal blur
    private var bloomHorizontalFramebuffer: Framebuffer

    /// A texture where the bloom horizontal blur is renderered to
    private var bloomHorizontalTexture: Texture2D

    // MARK: - Shaders

    /// A shader used to construct the GBuffer
    private let gbufferShader: ShaderProgram

    /// A shader used to execute lighting calculations for point lights
    private let pointLightShader: ShaderProgram

    /// A shader used to execute lighting calculations for spot lights
    private let spotLightShader: ShaderProgram

    /// A shader used to execute lighting calculations for directional lights
    private let directionalLightShader: ShaderProgram

    private let spotLightShadowMapShader: ShaderProgram

    /// A shader used to extract the over-exposed areas of a image to create
    /// the bloom effect.
    private let bloomExposureShader: ShaderProgram

    /// A shader used to perform gaussian blur on the over-exposed areas of
    /// a image.
    private let bloomBlurShader: ShaderProgram

    /// A shader that merges both the Bloom (if enabled) and applies HDR
    /// color corrections to the image. This shader also reverts the gamma
    /// correction applied by the GBuffer shader.
    private let hdrShader: ShaderProgram

    /// A shader that renders a skybox
    private let skyboxShader: ShaderProgram

    /// A vertex buffer that renders a quad on the entire screen.
    private let screenVertexBuffer: VertexBuffer

    /// A vertex buffer that renders a quad on the entire screen.
    private let cubeVertexBuffer: VertexBuffer

    // DEBUG SHADERS
    /// A texture debug shader
    private let textureDebugShader: ShaderProgram

    /// A that that enables or disables the debug view
    public var isDebugViewEnabled: Bool = false

    /// A set of textures that will be shown on the screen
    public var debugTextures: [Texture2D] = []

    // MARK: - Deferred Rendering

    /// Creates a new deferred renderer technique  using the `renderer` as the backend
    /// engine. The rendering technique will output the final composition to the `outputFramebuffer´.
    /// If `outputFramebuffer´ is not set, it will default to the renderer default
    /// framebuffer.
    public init(renderer: Renderer, outputFramebuffer framebuffer: Framebuffer? = nil) {
        outputFramebuffer = framebuffer ?? renderer.defaultFramebuffer
        gbuffer = GBuffer(renderer: renderer, width: outputFramebuffer.width,
                height: outputFramebuffer.height)

        renderingFramebuffer = renderer.framebuffer.init(
                width: outputFramebuffer.width,
                height: outputFramebuffer.height,
                depthAttachment: renderer.renderBuffer.init(
                        width: outputFramebuffer.width,
                        height: outputFramebuffer.height,
                        format: .depth(precision: .float32)))
        lightingTexture = renderer.texture2D.init(
                width: outputFramebuffer.width,
                height: outputFramebuffer.height,
                format: .color(precision: .float16, channels: 4))
        renderingFramebuffer.colorAttachments = [lightingTexture]

        shadowMapFramebuffer = renderer.framebuffer.init(width: 1024, height: 1024)
        shadowMap = renderer.texture2D.init(
                width: shadowMapFramebuffer.width,
                height: shadowMapFramebuffer.height,
                format: .depth(precision: .float16))
        shadowMap.wrap = (.clampToBorder, .clampToBorder)
        shadowMap.borderColor = Color(1.0)
        shadowMapFramebuffer.depthAttachment = shadowMap
        shadowMapFramebuffer.depthTest = true
        shadowMapFramebuffer.blending = false
        shadowMapFramebuffer.faceCulling = false

        bloomVerticalFramebuffer = renderer.framebuffer.init(
                width: outputFramebuffer.width,
                height: outputFramebuffer.height)
        bloomVerticalTexture = renderer.texture2D.init(
                width: outputFramebuffer.width,
                height: outputFramebuffer.height,
                format: .color(precision: .float16, channels: 4))
        bloomVerticalFramebuffer.colorAttachments = [bloomVerticalTexture]
        bloomVerticalTexture.wrap = (.clampToBorder, .clampToBorder)

        bloomHorizontalFramebuffer = renderer.framebuffer.init(
                width: outputFramebuffer.width,
                height: outputFramebuffer.height)
        bloomHorizontalTexture = renderer.texture2D.init(
                width: outputFramebuffer.width,
                height: outputFramebuffer.height,
                format: .color(precision: .float16, channels: 4))
        bloomHorizontalFramebuffer.colorAttachments = [bloomHorizontalTexture]
        bloomHorizontalTexture.wrap = (.clampToBorder, .clampToBorder)

        gbufferShader = renderer.shaderProgram.factory.deferredGeometryShader!

        pointLightShader = renderer.shaderProgram.factory.deferredPointLightShader!
        spotLightShader = renderer.shaderProgram.factory.deferredSpotLightShader!
        directionalLightShader = renderer.shaderProgram.factory.deferredDirectionalLightShader!

        spotLightShadowMapShader = renderer.shaderProgram.factory.directionalShadowMapShader!

        bloomExposureShader = renderer.shaderProgram.factory.bloomExposureMappingShader!
        bloomBlurShader = renderer.shaderProgram.factory.bloomBlurShader!
        hdrShader = renderer.shaderProgram.factory.hdrShader!

        skyboxShader = renderer.shaderProgram.factory.skyboxShader!

        let vertex: [[Float]] = [
            /*Position*/    /*TexCoords*/
            [-1.0, 1.0, 0.0, 0.0, 1.0],
            [-1.0, -1.0, 0.0, 0.0, 0.0],
            [1.0, 1.0, 0.0, 1.0, 1.0],
            [1.0, -1.0, 0.0, 1.0, 0.0]
        ]

        screenVertexBuffer = renderer.vertexBuffer.init(mesh: SimpleMesh(vertices: vertex.map {
            return Vertex(position: Vector3<Float>($0[0], $0[1], $0[2]), texCoords: Vector2<Float>($0[3], $0[4]))
        }, indices: [0, 1, 2, 2, 1, 3]))

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

        textureDebugShader = renderer.shaderProgram.factory.debugTextureShader!
        debugTextures = [gbuffer.normalTexture, gbuffer.materialTexture, gbuffer.positionTexture]
    }

    /// Renders the scene.
    public func render(scene: Scene) {
        renderGBuffer(scene: scene)
        renderLights(scene: scene)
        renderSkybox(scene: scene)
        renderBloom(scene: scene)
        renderHDR(scene: scene)

        if isDebugViewEnabled {
            renderDebug()
        }
    }

    /// Renders the GBuffer for the scene.
    private func renderGBuffer(scene: Scene) {
        gbuffer.framebuffer.faceCulling = false

        gbuffer.framebuffer.active {
            gbuffer.framebuffer.clear(color: Color<Float>(red: 0.0, green: 0.0, blue: 0.0, alpha: 0.0))
            gbufferShader.active {
                let projection = scene.camera.projection
                let view = SGLMath.translate(scene.camera.view, scene.camera.position)

                gbufferShader["projection"] = Matrix4x4<Float>(projection)
                gbufferShader["view"] = Matrix4x4<Float>(view)
                gbufferShader["gamma"] = gamma

                gbufferShader["near"] = Float(scene.camera.nearPlane)
                gbufferShader["far"] = Float(scene.camera.farPlane)

                GameEngine.shared.entityManager.forEach { (renderable: Renderable) in
                    guard let material = renderable.model.material as? PhongMaterial else {
                        return
                    }

                    var textureSlot: Int = 0
                    if let texture = material.diffuseTexture {
                        texture.activate(slot: textureSlot)
                        gbufferShader["material.diffuse"] = textureSlot
                        textureSlot += 1
                    }
                    if let texture = material.specularMap {
                        texture.activate(slot: textureSlot)
                        gbufferShader["material.specular"] = textureSlot
                        textureSlot += 1
                    }
                    if let texture = material.normalMap {
                        texture.activate(slot: textureSlot)
                        gbufferShader["material.normal"] = textureSlot
                        gbufferShader["material.hasNormalMap"] = true
                        textureSlot += 1
                    } else {
                        gbufferShader["material.hasNormalMap"] = false
                    }
                    if let texture = material.emissionMap {
                        texture.activate(slot: textureSlot)

                        gbufferShader["material.emission"] = Float(100.0)
                        gbufferShader["material.emissionMap"] = textureSlot
                        gbufferShader["material.hasEmissionMap"] = true
                        textureSlot += 1
                    } else {
                        gbufferShader["material.hasEmissionMap"] = false
                    }

                    gbufferShader["material.shininess"] = Float(64.0)

                    let model = SGLMath.translate(Matrix4x4<Double>(), -scene.camera.position) * renderable.transform
                    gbufferShader["model"] = Matrix4x4<Float>(model)
                    gbufferShader["inversedTransposedModel"] = Matrix4x4<Float>(transpose(model.inverse))

                    gbufferShader["MVP"] = Matrix4x4<Float>(projection * view * model)

                    if let vertexBuffer = renderable.model.mesh.vertexBuffer {
                        vertexBuffer.draw()
                    }
                }
            }
        }
    }

    /// Renders all the lights in the scene.
    private func renderLights(scene: Scene) {
        renderingFramebuffer.faceCulling = false
        renderingFramebuffer.depthTest = false
        renderingFramebuffer.blending = true
        renderingFramebuffer.blendingMode = .mix(source: 0.5, destination: 0.5)

        renderingFramebuffer.active {
            renderingFramebuffer.clear(color: Color<Float>(red: 0.0, green: 0.0, blue: 0.0, alpha: 0.0))
        }

        for light in scene.lights {
            if let light = light as? PointLight {
//                var lightSpaceMatrix: Matrix4x4<Float>? = nil
                let shadows = false
                if light.shadows && shadows {
                    renderShadowMap(for: light, in: scene)
//                    shadows = true
                }

                renderingFramebuffer.active {
                    pointLightShader.active {
                        pointLightShader["gPosition"] = 0
                        pointLightShader["gNormal"] = 1
                        pointLightShader["gAlbedoSpec"] = 2

                        gbuffer.positionTexture.activate(slot: 0)
                        gbuffer.normalTexture.activate(slot: 1)
                        gbuffer.materialTexture.activate(slot: 2)

                        pointLightShader["light.position"] = Vector3<Float>(light.position - scene.camera.position)
                        pointLightShader["light.ambient"] = light.ambient ?? Color<Float>(0.0)
                        pointLightShader["light.diffuse"] = light.diffuse ?? Color<Float>(0.0)
                        pointLightShader["light.specular"] = light.specular ?? Color<Float>(0.0)

                        pointLightShader["light.constant"] = Float(light.constant)
                        pointLightShader["light.linear"] = Float(light.linear)
                        pointLightShader["light.quadratic"] = Float(light.quadratic)

                        pointLightShader["light.shadowOcclusionStrength"] = Float(light.shadowOcclusionStrength)

                        pointLightShader["farPlane"] = Float(scene.camera.farPlane)
                        if shadows {
                            pointLightShader["shadowMap"] = 3
                            pointLightShader["hasShadowMap"] = true
                        } else {
                            pointLightShader["hasShadowMap"] = false
                        }

                        pointLightShader["camera.position"] = Vector3<Float>(scene.camera.viewPosition - scene.camera.position)

                        screenVertexBuffer.draw()
                    }
                }
            } else if let light = light as? DirectionalLight {
                renderingFramebuffer.active {
                    directionalLightShader.active {
                        directionalLightShader["gPosition"] = 0
                        directionalLightShader["gNormal"] = 1
                        directionalLightShader["gAlbedoSpec"] = 2

                        gbuffer.positionTexture.activate(slot: 0)
                        gbuffer.normalTexture.activate(slot: 1)
                        gbuffer.materialTexture.activate(slot: 2)

                        directionalLightShader["light.direction"] = Vector3<Float>(light.direction);
                        directionalLightShader["light.ambient"] = light.ambient ?? Color<Float>(0.0)
                        directionalLightShader["light.diffuse"] = light.diffuse ?? Color<Float>(0.0)
                        directionalLightShader["light.specular"] = light.specular ?? Color<Float>(0.0)

                        directionalLightShader["light.shadowOcclusionStrength"] = Float(light.shadowOcclusionStrength);

                        directionalLightShader["camera.position"] = Vector3<Float>(scene.camera.viewPosition - scene.camera.position)

                        screenVertexBuffer.draw()
                    }
                }
            } else if let light = light as? SpotLight {
                var lightSpaceMatrix: Matrix4x4<Double>? = nil
                if light.shadows && shadows {
                    lightSpaceMatrix = renderShadowMap(for: light, in: scene)
                }

                renderingFramebuffer.active {
                    spotLightShader.active {
                        spotLightShader["gPosition"] = 0
                        spotLightShader["gNormal"] = 1
                        spotLightShader["gAlbedoSpec"] = 2

                        gbuffer.positionTexture.activate(slot: 0)
                        gbuffer.normalTexture.activate(slot: 1)
                        gbuffer.materialTexture.activate(slot: 2)

                        spotLightShader["light.position"] = Vector3<Float>(light.position - scene.camera.position)
                        spotLightShader["light.direction"] = Vector3<Float>(light.direction)
                        spotLightShader["light.cutOff"] = Float(cos(radians(light.cutOff)))
                        spotLightShader["light.outerCutOff"] = Float(cos(radians(light.outerCutOff)))

                        spotLightShader["light.ambient"] = light.ambient ?? Color<Float>(0.0)
                        spotLightShader["light.diffuse"] = light.diffuse ?? Color<Float>(0.0)
                        spotLightShader["light.specular"] = light.specular ?? Color<Float>(0.0)

                        spotLightShader["light.constant"] = Float(light.constant)
                        spotLightShader["light.linear"] = Float(light.linear)
                        spotLightShader["light.quadratic"] = Float(light.quadratic)

                        spotLightShader["light.shadowOcclusionStrength"] = Float(light.shadowOcclusionStrength);

                        if let lightSpaceMatrix = lightSpaceMatrix {
                            spotLightShader["lightSpaceMatrix"] = Matrix4x4<Float>(lightSpaceMatrix);
                            spotLightShader["hasShadowMap"] = true;
                            spotLightShader["shadowMap"] = 3;
                            shadowMap.activate(slot: 3)
                        } else {
                            spotLightShader["hasShadowMap"] = false;
                        }

                        spotLightShader["camera.position"] = Vector3<Float>(scene.camera.viewPosition - scene.camera.position)

                        screenVertexBuffer.draw()
                    }
                }
            } else {
                fatalError("Lights of type \(type(of: light)) are not supported by the Deferred Renderer!")
            }
        }
    }

    /// Renders the shadow map for the given point light
    private func renderShadowMap(for light: PointLight, in scene: Scene) {

    }

    private func renderShadowMap(for light: SpotLight, in scene: Scene) -> Matrix4x4<Double> {
        let lightProjection = SGLMath.perspective(radians(90.0), 1.0, scene.camera.nearPlane, scene.camera.farPlane)
        let lightView = SGLMath.lookAt(
                light.position,
                light.position + light.direction,
                Vector3<Double>(0, 1, 0)
        )
        let lightSpaceMatrix = lightProjection * lightView

        shadowMapFramebuffer.active {
            shadowMapFramebuffer.clear(color: nil)

            spotLightShadowMapShader.active {
                spotLightShadowMapShader["lightSpaceMatrix"] = Matrix4x4<Float>(lightSpaceMatrix);
                spotLightShadowMapShader["near"] = Float(scene.camera.nearPlane);
                spotLightShadowMapShader["far"] = Float(scene.camera.farPlane);

                GameEngine.shared.entityManager.forEach { (renderable: Renderable) in
                    if renderable.castShadows {
                        let model = renderable.transform
                        spotLightShadowMapShader["model"] = Matrix4x4<Float>(model)
                        if let vertexBuffer = renderable.model.mesh.vertexBuffer {
                            vertexBuffer.draw()
                        }
                    }
                }
            }
        }

        return lightSpaceMatrix
    }

    /// Renders the Skybox if a skybox texture is set
    private func renderSkybox(scene: Scene) {
        if let backgroundSkybox = backgroundSkybox {
            renderingFramebuffer.depthTest = true
            renderingFramebuffer.blending = false

            renderingFramebuffer.depthTestingMode = .lessThanOrEqual

            // To render the skybox we need the gbuffer depth information
            gbuffer.framebuffer.copyDepth(destination: renderingFramebuffer)
            renderingFramebuffer.active {
                skyboxShader.active {
                    skyboxShader["skybox"] = 0
                    backgroundSkybox.activate(slot: 0)

                    skyboxShader["gamma"] = Float(gamma)
                    skyboxShader["projection"] = Matrix4x4<Float>(scene.camera.projection)
                    skyboxShader["view"] = Matrix4x4<Float>(scene.camera.view)

                    cubeVertexBuffer.draw()
                }
            }
        }
    }

    /// Renders the Bloom blur effect
    private func renderBloom(scene: Scene) {
        if bloom == false {
            return
        }

        bloomVerticalFramebuffer.faceCulling = false
        bloomVerticalFramebuffer.depthTest = false
        bloomVerticalFramebuffer.blending = false
        bloomVerticalFramebuffer.clear(color: nil)

        bloomHorizontalFramebuffer.faceCulling = false
        bloomHorizontalFramebuffer.depthTest = false
        bloomHorizontalFramebuffer.blending = false
        bloomHorizontalFramebuffer.clear(color: nil)

        bloomVerticalFramebuffer.active {
            bloomExposureShader.active {
                bloomExposureShader["scene"] = 0
                lightingTexture.activate(slot: 0)
                screenVertexBuffer.draw()
            }
        }

        bloomBlurShader.active {
            for _ in 0..<bloomAmount {
                bloomHorizontalFramebuffer.active {
                    bloomBlurShader["horizontal"] = true
                    bloomVerticalTexture.activate(slot: 0)
                    screenVertexBuffer.draw()
                }

                bloomVerticalFramebuffer.active {
                    bloomBlurShader["horizontal"] = false
                    bloomHorizontalTexture.activate(slot: 0)
                    screenVertexBuffer.draw()
                }
            }
        }
    }

    /// Renders the final HDR pass
    private func renderHDR(scene: Scene) {
        outputFramebuffer.faceCulling = false
        outputFramebuffer.depthTest = false
        outputFramebuffer.blending = false

        outputFramebuffer.active {
            hdrShader.active {
                lightingTexture.activate(slot: 0)
                bloomVerticalTexture.activate(slot: 1)

                hdrShader["exposure"] = scene.exposure
                hdrShader["gamma"] = gamma
                hdrShader["scene"] = 0
                hdrShader["bloom"] = 1
                hdrShader["hasHDR"] = hdr
                hdrShader["hasBloom"] = bloom

                screenVertexBuffer.draw()
            }
        }
    }

    /// Renders the debug pass
    private func renderDebug() {
        outputFramebuffer.active {
            textureDebugShader.active {
                let textureSize: Float = min(1.0 / Float(debugTextures.count), 0.25)
                for (i, texture) in debugTextures.enumerated() {
                    let transform = SGLMath.scale(
                            SGLMath.translate(Matrix4x4<Float>(), Vector3<Float>(
                                    x: 1.0 - textureSize,
                                    y: 1.0 - textureSize * 2 * Float(i + 1) + textureSize,
                                    z: 0.0)),
                            Vector3<Float>(textureSize)
                    )

                    textureDebugShader["model"] = transform
                    textureDebugShader["image"] = 0
                    texture.activate(slot: 0)

                    screenVertexBuffer.draw()
                }
            }
        }
    }
}

public class GBuffer {

    /// A framebuffer used to construct the geometry buffers
    var framebuffer: Framebuffer

    /// A texture holding the fragment world position
    let positionTexture: Texture2D

    /// A texture holding normals
    let normalTexture: Texture2D

    /// A texture holding material data: diffuse colors and specular factor
    let materialTexture: Texture2D

    public init(renderer: Renderer, width: UInt, height: UInt) {
        framebuffer = renderer.framebuffer.init(
                width: width, height: height,
                depthAttachment: renderer.renderBuffer.init(
                        width: width, height: height,
                        format: .depth(precision: .float32)))
        framebuffer.faceCulling = false
        framebuffer.depthTest = true

        positionTexture = renderer.texture2D.init(width: width, height: height,
                format: .color(precision: .float32, channels: 4))
        normalTexture = renderer.texture2D.init(width: width, height: height,
                format: .color(precision: .float32, channels: 4))
        materialTexture = renderer.texture2D.init(width: width, height: height,
                format: .color(precision: .float16, channels: 4))

        framebuffer.colorAttachments = [
            positionTexture,
            normalTexture,
            materialTexture
        ]
    }

}

public protocol DeferredRenderingShaderProgramFactory: ShaderProgramFactory {
    /// The Deferred Point Light shader program
    var deferredPointLightShader: ShaderProgram? { get }

    /// The Deferred Spot Light shader program
    var deferredSpotLightShader: ShaderProgram? { get }

    /// The Deferred Geometry shader program
    var deferredGeometryShader: ShaderProgram? { get }

    /// The Deferred Directional Light shader program
    var deferredDirectionalLightShader: ShaderProgram? { get }
}

fileprivate extension ShaderProgramFactory {

    /// The Deferred Point Light shader program
    var deferredPointLightShader: ShaderProgram? {
        return self.for(DeferredRenderingShaderProgramFactory.self)?.deferredPointLightShader
    }

    /// The Deferred Spot Light shader program
    var deferredSpotLightShader: ShaderProgram? {
        return self.for(DeferredRenderingShaderProgramFactory.self)?.deferredSpotLightShader
    }

    /// The Deferred Geometry shader program
    var deferredGeometryShader: ShaderProgram? {
        return self.for(DeferredRenderingShaderProgramFactory.self)?.deferredGeometryShader
    }

    /// The Deferred Directional Light shader program
    var deferredDirectionalLightShader: ShaderProgram? {
        return self.for(DeferredRenderingShaderProgramFactory.self)?.deferredDirectionalLightShader
    }

}
