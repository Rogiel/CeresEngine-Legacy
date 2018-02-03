//
//  GameEngine.swift
//  GameEnginePackageDescription
//
//  Created by Rogiel Sulzbach on 10/2/17.
//

import Foundation
@_exported import SGLMath
@_exported import SGLImage

public class GameEngine {
    
    /// The engine's entity manager
    public let entityManager: EntityManager = EntityManager()

    /// The scene to be rendered by the engine
    public var scene: Scene

    /// The renderer to be used to render the game scene
    public let renderer: Renderer

    /// The rendering technique to be used by the engine
    public var renderingTechnique: RenderingTechnique

    /// The engine's InputController
    public var inputController: InputController?

    /// Initializes the game engine with the `renderer`.
    ///
    /// - Parameter renderer: the renderer to be used  by the engine.
    public init(renderer: Renderer, inputController: InputController? = nil) {
        self.renderer = renderer
        self.renderingTechnique = PBRDeferredRenderingTechnique(renderer: renderer)
        self.scene = Scene(camera: FirstPersonCamera())
        self.inputController = inputController
        GameEngine.shared = self
    }
    
    /// The game engine shared instance
    public private(set) static var shared: GameEngine! = nil;

    private var last: Date = Date()

    /// Renders the main scene.
    ///
    /// - Parameters timeScale:     the rendering time scale
    public func render(timeScale: TimeInterval = 1.0) -> TimeInterval {
        let current = Date()
        let interval = current.timeIntervalSince(last)
        last = current

        entityManager.forEach { (entity: Updatable) in
            entity.update(delta: interval * timeScale)
        }

        // Render
        renderingTechnique.render(scene: scene)

        return interval
    }

}

/// A protocol that must be implemented by Game bundles 
protocol Game {

    /// Update the game entities
    func update(deltaTime time: Double)

}
