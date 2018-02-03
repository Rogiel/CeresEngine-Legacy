//
//  Renderable.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/1/17.
//  Copyright © 2017 Rogiel Sulzbach. All rights reserved.
//

public protocol Renderable : Transformable {
    
    /// The renderable model
    var model: Model { get }
    
    var castShadows: Bool { get }
    
}
