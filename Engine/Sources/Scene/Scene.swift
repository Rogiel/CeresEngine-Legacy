//
//  Scene.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

public class Scene {
    
    /// The list of all lights in the scene
    public var lights: [Light] = []

    /// The scene camera
    public var camera: Camera
    
    /// The scene HDR exposure level. If HDR is disabled, this value
    /// is ignored.
    public var exposure: Float = 1.0
    
    /// Creates a new scene
    public init(camera: Camera) {
        self.camera = camera
    }
    
}

public class SceneNode {
    
    
    
}
