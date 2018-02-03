//
//  RenderingTechnique.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

public protocol RenderingTechnique : class {

    /// Renders a scene.
    ///
    /// - Parameter scene: the scene to be rendered
    func render(scene: Scene)
    
}
