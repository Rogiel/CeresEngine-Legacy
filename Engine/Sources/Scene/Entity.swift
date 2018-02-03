//
//  Tratis.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/1/17.
//  Copyright © 2017 Rogiel Sulzbach. All rights reserved.
//

import SGLMath

public protocol Entity : class {
    
    /// The entity manager that currently owns this entity
    var entityManager: EntityManager? { get }
    
    /// A numeric value that represents the entity ID
    var entityID: EntityID? { get }

    /// A notification sent to a entity by a `EntityManager` to notify
    /// that it has been registered to a `EntityManager`
    func didRegisterWith(entityManager: EntityManager, entityID: EntityID)
    
    /// Destroys the entity.
    ///
    /// Upon destruction, the entity will automatically be removed
    /// from its entity manager
    func destroy()
    
}

/// A numeric type that represents the entity ID
public typealias EntityID = Int

open class BaseEntity : Entity {
    
    /// The entity manager that currently owns this entity
    public private(set) var entityManager: EntityManager?
    
    /// A numeric value that represents the entity ID
    public private(set) var entityID: EntityID? = 0

    public init() {
        
    }
    
    /// A notification sent to a entity by a `EntityManager` to notify
    /// that it has been registered to the `EntityManager`
    public func didRegisterWith(entityManager: EntityManager, entityID: EntityID) {
        self.entityManager = entityManager
        self.entityID = entityID
    }
    
    /// Destroys the entity.
    ///
    /// Upon destruction, the entity will automatically be removed
    /// from its entity manager
    public func destroy() {
        entityManager?.unregister(entity: self)
    }
    
}

public class EntityManager {
    
    /// A list of all entities registered with the manager
    public private(set) var entities: [Entity] = []
    
    public init() {
        
    }
    
    /// Registers a new entity
    public func register(entity: Entity) {
        let index = entities.endIndex
        entity.didRegisterWith(entityManager: self, entityID: index)
        entities.insert(entity, at: index)
    }
    
    /// Unregisters a entity
    public func unregister(entity: Entity) {
        if let index = entity.entityID {
            entities.remove(at: index)
        }
    }
    
    /// Applies a function for each entity that matches the type `T`.
    ///
    /// - Parameter executor:   the callback to be called for each entity
    ///                         that matches the type `T`.
    public func forEach<T>(executor: (_ entity: T) -> Void) {
        for entity in entities {
            if let machingEntity = entity as? T {
                executor(machingEntity)
            }
        }
    }
    
}

public protocol Transformable : Entity {
    /// The object transform matrix
    var transform: Matrix4x4<Double> { get }

    /// The object parent transformable object
    var parent: Transformable? { get }

    /// The object's parent transform matrix
    var parentTransform: Matrix4x4<Double>? { get }
}

public extension Transformable {

    /// The object's parent transform matrix
    public var parentTransform: Matrix4x4<Double>? { return parent?.transform }

}

/// Represents a entity type that has a position
public protocol Positioning : Transformable {
    /// The object position
    var position: Vector3<Double> { get }

    /// The object position in world space
    var worldPosition: Vector3<Double> { get }
}

public protocol Positionable : Positioning {
    /// The object position in its local space
    var position: Vector3<Double> { get set }

    /// The object position in world space
    var worldPosition: Vector3<Double> { get set }
}

public extension Positioning  {
    public var transform: Matrix4x4<Double> {
        return SGLMath.translate(Matrix4x4<Double>(), position)
    }

    /// The object position in world space
    public var worldPosition: Vector3<Double> {
        return ((parent as? Positioning)?.worldPosition ?? Vector3<Double>(0.0)) + position
    }
}

/// Represents a entity type that has a rotation
public protocol Rotating : Transformable {
    /// The object rotation in its local space
    var rotation: Vector3<Double> { get }

    /// The object position in world space
    var worldRotation: Vector3<Double> { get }
}

public protocol Rotatable : Rotating {
    /// The object rotation in its local space
    var rotation: Vector3<Double> { get set }

    /// The object rotation in world space
    var worldRotation: Vector3<Double> { get set }
}

public extension Rotating {
    public var transform: Matrix4x4<Double> {
        return
            SGLMath.rotate(
                SGLMath.rotate(
                    SGLMath.rotate(
                        Matrix4x4<Double>(),
                        rotation.x, Vector3<Double>(1.0, 0.0, 0.0)
                    ),
                    rotation.y, Vector3<Double>(0.0, 1.0, 0.0)
                ),
                rotation.z, Vector3<Double>(0.0, 0.0, 1.0)
        )
    }

    /// The object rotation in world space
    public var worldRotation: Vector3<Double> {
        return ((parent as? Rotating)?.worldRotation ?? Vector3<Double>(0.0)) + rotation
    }
}

/// Represents a entity type that has a scale
public protocol Scaling : Transformable {
    /// The object scale in its local space
    var scale: Vector3<Double> { get }

    /// The object scale in world space
    var worldScale: Vector3<Double> { get }
}

public protocol Scalable : Scaling {
    /// The object scale in its local space
    var scale: Vector3<Double> { get set }

    /// The object scale in world space
    var worldScale: Vector3<Double> { get set }
}

public extension Scaling {
    var transform: Matrix4x4<Double> {
        return SGLMath.scale(Matrix4x4<Double>(), scale)
    }

    /// The object scale in world space
    public var worldScale: Vector3<Double> {
        return ((parent as? Scaling)?.worldScale ?? Vector3<Double>(1.0)) * scale
    }
}

/// Represents a entity type that can be updated
public protocol Updatable : Entity {
    
    /// Updates the entity
    func update(delta: Double)
    
}
