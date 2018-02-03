//
//  Light.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import SGLMath

public class Light {
    /// The light ambient color
    public var ambient: Color<Float>?
    
    /// The light diffuse color
    public var diffuse: Color<Float>?
    
    /// The light specular color
    public var specular: Color<Float>?
    
    /// A flag that indicates if the light should cast shadows
    public var shadows: Bool = true;
    
    /// A constant that control the shadow occlusion strength
    public var shadowOcclusionStrength: Double = 0.5;
    
    /// Creates a new light
    public init() {
        
    }
}

public class PointLight : Light {
    /// The point light position
    public var position: Vector3<Double> = Vector3<Double>(0.0)
    
    /// The point light constant attenuation constant
    public var constant: Double = 0.0;
    
    /// The point light linear attenuation constant
    public var linear: Double = 1.0;
    
    /// The point light quadratic attenuation constant
    public var quadratic: Double = 0.0;
        
    /// Creates a new point light
    public override init() {
        
    }
}

public class DirectionalLight : Light {
    /// The directional light direction
    public var direction: Vector3<Double> = Vector3<Double>(x: 0.0, y: 0.0, z: 0.0);
    
    /// Creates a new directional light
    public override init() {
        
    }
}

public class SpotLight : Light {
    /// The point light position
    public var position: Vector3<Double> = Vector3<Double>(0.0)
    
    /// The spotlight direction
    public var direction: Vector3<Double> = Vector3<Double>(x: 0.0, y: 0.0, z: 0.0);
    
    /// The spot light cutoff angle
    public var cutOff: Double = 10.0;
    
    /// The spot light outer cutoff angle
    public var outerCutOff: Double = 15.0;
    
    /// The spot light constant attenuation constant
    public var constant: Double = 0.0;
    
    /// The spot light linear attenuation constant
    public var linear: Double = 1.0;
    
    /// The spot light quadratic attenuation constant
    public var quadratic: Double = 0.0;
    
    /// Creates a new spot light
    public override init() {
        
    }
}
