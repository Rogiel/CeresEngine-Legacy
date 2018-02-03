//
//  Model.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import Foundation
import SGLImage

public protocol Model {

    /// The model mesh
    var mesh: Mesh { get }

    /// The model material
    var material: Material? { get }

}

public class SimpleModel: Model {

    /// The model mesh
    public var mesh: Mesh

    /// The model material
    public var material: Material?

    /// Creates a new SimpleModel object
    public init(mesh: Mesh,
                material: Material? = nil) {
        self.mesh = mesh
        self.material = material
    }

}

public class JSONSceneLoader {

    public struct SceneData: Codable {
        public struct ModelData: Codable {
            public var name: String
            public var mesh: String

            public struct PBRMaterial: Codable {
                public var type: String

                public var albedo: String?
                public var metallic: String?
                public var roughness: String?
                public var normal: String?
                public var ambientOcclusion: String?
                public var displacement: String?
                public var maximumDisplacement: Float = 0.0
                public var emission: String?
            }

            public var material: PBRMaterial
        }

        public var name: String
        public var objects: [ModelData] = []
    }

    /// Loads the resource
    public func load(resource: Resource) throws -> [Model] {
        guard let resource = resource as? FileResource else {
            throw ResourceLoadingError.invalidResourceType
        }

        let decoder = JSONDecoder()
        let sceneData = try decoder.decode(SceneData.self, from: resource.data!)

        print("Loading \(resource.url.path)")
        print("\(sceneData)")

        var models: [Model] = []
        for modelDescriptor in sceneData.objects {
            func loadImage(_ path: String?) -> Texture2D? {
                if let path = path,
                   let image = try? resource.resourceManager.load(named: path) as ImageRGBA {
                    return GameEngine.shared.renderer.texture2D.init(image: image)
                }
                return nil
            }

            models += [SimpleModel(
                    mesh: try! resource.resourceManager.load(named: modelDescriptor.mesh),
                    material: PBRMaterial(
                            albedoMap: loadImage(modelDescriptor.material.albedo),
                            normalMap: loadImage(modelDescriptor.material.normal),
                            metallicMap: loadImage(modelDescriptor.material.metallic),
                            roughnessMap: loadImage(modelDescriptor.material.roughness),
                            ambientOcclusionMap: loadImage(modelDescriptor.material.ambientOcclusion),
                            displacementMap: loadImage(modelDescriptor.material.displacement),
                            maximumDisplacement: modelDescriptor.material.maximumDisplacement,
                            emissionMap: loadImage(modelDescriptor.material.emission)))]
        }
        return models
    }
}