//
// Created by Rogiel Sulzbach on 10/29/17.
//

public protocol Material {

    /// Binds the material attributes to the shader program
    func bind(shaderProgram: ShaderProgram)

}

public class PhongMaterial: Material {

    /// The model diffuse texture
    public var diffuseTexture: Texture2D?

    /// The model specular map texture
    public var specularMap: Texture2D?

    /// The model normal map texture
    public var normalMap: Texture2D?

    /// The model emission map texture
    public var emissionMap: Texture2D?

    public init(diffuseTexture: Texture2D? = nil,
                specularMap: Texture2D? = nil,
                normalMap: Texture2D? = nil,
                emissionMap: Texture2D? = nil) {
        self.diffuseTexture = diffuseTexture
        self.specularMap = specularMap
        self.normalMap = normalMap
        self.emissionMap = emissionMap
    }

    /// Binds the material attributes to the shader program
    public func bind(shaderProgram: ShaderProgram) {
        shaderProgram["material.diffuseTexture"] = 0
        diffuseTexture?.activate(slot: 0)

        shaderProgram["material.specularMap"] = 1
        specularMap?.activate(slot: 1)

        shaderProgram["material.normalMap"] = 2
        normalMap?.activate(slot: 2)

        shaderProgram["material.emissionMap"] = 3
        emissionMap?.activate(slot: 3)
    }
}

public class PBRMaterial: Material {

    /// The material albedo map
    public var albedoMap: Texture2D?

    /// The material normal map
    public var normalMap: Texture2D?

    /// The material metallic map
    public var metallicMap: Texture2D?

    /// The material roughness map
    public var roughnessMap: Texture2D?

    /// The material ambient occlusion map
    public var ambientOcclusionMap: Texture2D?

    /// The material displacement map
    public var displacementMap: Texture2D?

    public var maximumDisplacement: Float

    /// The material emission map
    public var emissionMap: Texture2D?

    /// Creates a new PBR material
    public init(
            albedoMap: Texture2D?, normalMap: Texture2D? = nil,
            metallicMap: Texture2D?, roughnessMap: Texture2D?,
            ambientOcclusionMap: Texture2D? = nil,
            displacementMap: Texture2D? = nil, maximumDisplacement: Float? = nil,
            emissionMap: Texture2D? = nil) {
        self.albedoMap = albedoMap
        self.normalMap = normalMap
        self.metallicMap = metallicMap
        self.roughnessMap = roughnessMap
        self.ambientOcclusionMap = ambientOcclusionMap
        self.displacementMap = displacementMap
        self.maximumDisplacement = maximumDisplacement ?? 0.0
        self.emissionMap = emissionMap
    }

    /// A list of all texture maps
    public var textureMap: [String: Texture?] {
        return [
            "albedoMap": albedoMap,
            "normalMap": normalMap,
            "metallicMap": metallicMap,
            "roughnessMap": roughnessMap,
            "ambientOcclusionMap": ambientOcclusionMap,
            "emissionMap": emissionMap,
            "displacementMap": displacementMap
        ]
    }

    public var uniformMap: [String: Uniformable] {
        return [
            "maximumDisplacement": maximumDisplacement
        ]
    }

    /// Binds the material attributes to the shader program
    public func bind(shaderProgram: ShaderProgram) {
        var textureSlot: Int = 0
        for (name, texture) in textureMap {
            if let texture = texture {
                texture.activate(slot: textureSlot)
                shaderProgram["material.\(name)"] = textureSlot
                textureSlot += 1
            }
        }
        shaderProgram.setUniforms(prefix: "material.", uniformMap)
    }

}