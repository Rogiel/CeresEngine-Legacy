//
//  Texture.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import SGLImage

/// A protocol that encapsulates the common behaviour expected of all texture
/// types.
public protocol Texture : RenderTarget {

    /// Activates the texture object
    func activate(slot: Int?)
    
    /// Deactivates the texture object
    func deactivate()
    
    /// Executes `body` within the context of the framebuffer.
    func active(_ slot: Int, body: () throws -> Void) rethrows
    
    /// Executes `body` within the context of the framebuffer.
    func active(body: () throws -> Void) rethrows
    
    // MARK: - Operations
    
    /// Generate mipmaps for the texture.
    func generateMipmaps() throws
    
    // MARK: - Options
    
    /// The texture format
    var format: TextureFormat { get }
    
    /// Sets the texture border color. Used when the wrapping mode
    /// is set to `.clampToBorder`.
    var borderColor: Color<Float> { get set }
    
    /// Sets the texture magnification and minification function is used
    /// whenever the level-of-detail function used when sampling from
    /// the texture determines that the texture should be magified or
    /// minified.
    var magnification: TextureMagnification { get set }
    
    /// Sets the texture minifying function is used by the level-of-detail
    /// function when sampling from the texture determines that the
    /// texture should be minified.
    var minification: TextureMinification { get set }

    /// Returns a texture at the given mipmap level
    subscript(level level: TextureLevelIndex) -> TextureLevel { get }

}

/// A type that represents a texture dimension
public typealias TextureDimension = UInt

/// A type type that represents the index of a texture level
public typealias TextureLevelIndex = UInt

/// A type that represents the type index of a texture layer
public typealias TextureLayerIndex = UInt

/// A single dimensional texture.
public protocol Texture1D : Texture {

    /// Creates a new texture with the given format and size
    init(width: TextureDimension, format: TextureFormat)

    /// The texture width
    var width: TextureDimension { get set }

    /// Sets the wrap mode for the texture
    var wrap: TextureWrap { get set }

}

public extension Texture1D {
    /// Sets the wrap mode for the texture on the S axis
    public var wrapS: TextureWrap {
        get { return wrap }
        set { wrap = newValue }
    }
}

/// A two dimensional texture.
public protocol Texture2D : Texture {

    /// Creates a new texture with the given format and size
    init(width: TextureDimension, height: TextureDimension, format: TextureFormat)

    /// Creates a new texture object from the contents of a image
    init(image: Image)

    /// The texture width
    var width: TextureDimension { get set }

    /// The texture height
    var height: TextureDimension { get set }

    /// Sets the wrap mode for the texture
    var wrap: (s: TextureWrap, t: TextureWrap) { get set }

    /// Update a texture image data
    ///
    /// - Parameter image: the new image data to assign to the texture
    func update(image: Image)

}

public extension Texture2D {
    /// Sets the wrap mode for the texture on the S axis
    public var wrapS: TextureWrap {
        get { return wrap.s }
        set { wrap.s = newValue }
    }

    /// Sets the wrap mode for the texture on the T axis
    public var wrapT: TextureWrap {
        get { return wrap.t }
        set { wrap.t = newValue }
    }
}

/// A three dimensional texture.
public protocol Texture3D: Texture {

    /// Creates a new texture with the given format and size
    init(width: TextureDimension, height: TextureDimension,
         depth: TextureDimension, format: TextureFormat)

    /// The texture width
    var width: TextureDimension { get set }

    /// The texture height
    var height: TextureDimension { get set }

    /// The texture depth
    var depth: TextureDimension { get set }

    /// Sets the wrap mode for the texture
    var wrap: (r: TextureWrap, s: TextureWrap, t: TextureWrap) { get set }

    /// Returns a texture at the given mipmap level
    subscript(layer layer: TextureLayerIndex,
              level level: TextureLevelIndex) -> Texture3DLayer { get }

}

public extension Texture3D {
    /// Sets the wrap mode for the texture on the R axis
    public var wrapR: TextureWrap {
        get { return wrap.r }
        set { wrap.r = newValue }
    }

    /// Sets the wrap mode for the texture on the S axis
    public var wrapS: TextureWrap {
        get { return wrap.s }
        set { wrap.s = newValue }
    }

    /// Sets the wrap mode for the texture on the T axis
    public var wrapT: TextureWrap {
        get { return wrap.t }
        set { wrap.t = newValue }
    }

    /// Returns a texture at the given mipmap level
    public subscript(layer layer: TextureLayerIndex) -> Texture3DLayer {
        return self[layer: layer, level: 0]
    }
}

public protocol Texture3DLayer : TextureLevel {
    /// The 3D texture layer index
    var layer: TextureLayerIndex { get }

    /// The texture width
    var width: TextureDimension { get }

    /// The texture height
    var height: TextureDimension { get }
}

/// A Cubemap texture. Each face of the texture can be
/// remapped into a 2D texture
public protocol TextureCube: Texture {

    /// Creates a new texture with the given format and size
    init(width: TextureDimension, height: TextureDimension, format: TextureFormat)

    /// Creates a new texture object from the contents of a image
    init(positiveX: Image, negativeX: Image,
         positiveY: Image, negativeY: Image,
         positiveZ: Image, negativeZ: Image)

    /// The texture width
    var width: TextureDimension { get set }

    /// The texture height
    var height: TextureDimension { get set }

    /// Sets the wrap mode for the texture
    var wrap: (r: TextureWrap, s: TextureWrap, t: TextureWrap) { get set }

    /// The the cube's positive-x face
    var positiveX: TextureCubeFace { get }

    /// The the cube's negative-x face
    var negativeX: TextureCubeFace { get }

    /// The the cube's positive-y face
    var positiveY: TextureCubeFace { get }

    /// The the cube's negative-y face
    var negativeY: TextureCubeFace { get }

    /// The the cube's positive-z face
    var positiveZ: TextureCubeFace { get }

    /// The the cube's negative-z face
    var negativeZ: TextureCubeFace { get }

}

public extension TextureCube {
    /// Sets the wrap mode for the texture on the R axis
    public var wrapR: TextureWrap {
        get { return wrap.r }
        set { wrap.r = newValue }
    }

    /// Sets the wrap mode for the texture on the S axis
    public var wrapS: TextureWrap {
        get { return wrap.s }
        set { wrap.s = newValue }
    }

    /// Sets the wrap mode for the texture on the T axis
    public var wrapT: TextureWrap {
        get { return wrap.t }
        set { wrap.t = newValue }
    }

    /// A array of all texture cube faces
    public var faces: [TextureCubeFace] {
        return [positiveX, negativeX,
                positiveY, negativeY,
                positiveZ, negativeZ]
    }
}

public protocol TextureCubeFace : RenderTarget {
    /// The texture width
    var width: TextureDimension { get }

    /// The texture height
    var height: TextureDimension { get }

    /// Returns a texture at the given mipmap level
    subscript(level level: TextureLevelIndex) -> TextureCubeFace { get }
}

public protocol TextureLevel : RenderTarget {
    /// The parent texture
    var texture: Texture { get }

    /// The texture level
    var level: TextureLevelIndex { get }
}

/// The texture format
public enum TextureFormat {
    
    /// A texture that has a color buffer with up to 4 color channels.
    case color(precision: TexturePrecision, channels: Int)
    
    /// A texture that has a depth buffer attached to it.
    case depth(precision: TexturePrecision)
    
}

/// The texture format precision
public enum TexturePrecision {
    
    /// A texture that represents each channel using 8 bits
    case uint8
    
    /// A texture that represents each channel using 16 bit floats
    case float16

    /// A texture that represents each channel using 32 bit floats
    case float32
    
}

/// The texture wrap mode
public enum TextureWrap {
    
    /// Causes ss coordinates to be clamped to the range [1/2N, 1 − 1/2N],
    /// where N is the size of the texture in the direction of clamping
    ///
    /// The texture coordinate is clamped to the [0, 1] range.
    case clampToEdge
    
    /// evaluates ss coordinates in a similar manner to CLAMP_TO_EDGE.
    /// However, in cases where clamping would have occurred in
    /// CLAMP_TO_EDGE mode, the fetched texel data is substituted with
    /// the values specified by TEXTURE_BORDER_COLOR
    ///
    /// The texture coordinate is clamped to the [0, 1] range, but the
    /// edge texels are blended with a constant border color, defined
    /// by `borderColor`.
    case clampToBorder
    
    /// Causes the integer part of the ss coordinate to be ignored; the
    /// GL uses only the fractional part, thereby creating a repeating
    /// pattern.
    ///
    /// The texture coordinate wraps around the texture. So a texture
    /// coordinate of -0.2 becomes the equivalent of 0.8.
    case `repeat`
    
    /// Causes the ss coordinate to be set to the fractional part of
    /// the texture coordinate if the integer part of ss is even;
    /// if the integer part of ss is odd, then the ss texture coordinate
    /// is set to 1−frac(s)1-frac⁡s, where frac(s)frac⁡s represents the
    /// fractional part of ss.
    ///
    /// The texture coordinate wraps around like a mirror. -0.2 becomes
    /// 0.2, -1.2 becomes 0.8, etc.
    case mirroredRepeat
    
    /// Causes the the ss coordinate to be repeated as for
    /// MIRRORED_REPEAT for one repetition of the texture, at which
    /// point the coordinate to be clamped as in CLAMP_TO_EDGE.
    ///
    /// The texture coordinates are clamped to the [-1, 1] range,
    /// but the negative coordinates are mirrors of the positive.
    /// This effectively makes the texture twice as big through
    /// mirroring, but clamps to the edge beyond that.
    case mirrorClampToEdge
    
}

/// The texture magnification mode
public enum TextureMagnification {
    
    /// Returns the value of the texture element that is nearest
    /// (in Manhattan distance) to the specified texture
    /// coordinates.
    case nearest
    
    /// Returns the weighted average of the four texture elements
    /// that are closest to the specified texture coordinates.
    /// These can include items wrapped or repeated from other
    /// parts of a texture, depending on the values of S and T
    /// wrapping, and on the exact mapping.
    case linear
    
}

/// The texture minification mode
public enum TextureMinification {
    
    /// Returns the value of the texture element that is nearest
    /// (in Manhattan distance) to the specified texture
    /// coordinates.
    case nearest
    
    /// Returns the weighted average of the four texture elements
    /// that are closest to the specified texture coordinates.
    /// These can include items wrapped or repeated from other
    /// parts of a texture, depending on the values of
    /// TEXTURE_WRAP_S and TEXTURE_WRAP_T, and on the exact
    /// mapping.
    case linear
    
    /// Chooses the mipmap that most closely matches the size of the
    /// pixel being textured and uses the NEAREST criterion (the
    /// texture element closest to the specified texture coordinates)
    /// to produce a texture value.
    case nearestMipmapNearest
    
    /// Chooses the mipmap that most closely matches the size of the
    /// pixel being textured and uses the LINEAR criterion (a weighted
    /// average of the four texture elements that are closest to the
    /// specified texture coordinates) to produce a texture value.
    case linearMipmapNearest
    
    /// Chooses the two mipmaps that most closely match the size of the
    /// pixel being textured and uses the NEAREST criterion (the texture
    /// element closest to the specified texture coordinates) to produce
    /// a texture value from each mipmap. The final texture value is a
    /// weighted average of those two values.
    case nearestMipmapLinear
    
    /// Chooses the two mipmaps that most closely match the size of the
    /// pixel being textured and uses the LINEAR criterion (a weighted
    /// average of the texture elements that are closest to the specified
    /// texture coordinates) to produce a texture value from each mipmap.
    /// The final texture value is a weighted average of those two values.
    case linearMipmapLinear
    
};

public extension Texture {
    
    public func active(_ slot: Int, body: () throws -> Void) rethrows {
        activate(slot: slot)
        try body()
        deactivate()
    }
    
    public func active(body: () throws -> Void) rethrows {
        activate(slot: nil)
        try body()
        deactivate()
    }
    
}
