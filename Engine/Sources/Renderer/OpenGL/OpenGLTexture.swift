//
//  OpenGLTexture.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import SGLImage
import SGLOpenGL

/// A type that identifies a OpenGL Texture object
public typealias OpenGLTextureID = GLuint

public class OpenGLTexture: Texture {

    /// The OpenGL texture object ID
    public let textureID: OpenGLTextureID

    /// The OpenGL texture type
    public let glTextureType: GLenum

    /// Creates a new OpenGL texture
    fileprivate init(type: GLenum, format: TextureFormat) {
        self.textureID = OpenGLTexture.allocate()
        self.glTextureType = type
        self.format = format
    }

    deinit {
        var textureID = self.textureID
        glDeleteTextures(1, &textureID)
    }

    private static func allocate() -> OpenGLTextureID {
        var id: OpenGLTextureID = 0
        glGenTextures(1, &id)
        return id
    }

    /// A variable holding the currently active slot for the texture.
    /// This slot is used on the `deactivate` method to restore to
    /// the initial state.
    fileprivate var currentActiveSlot: Int? = nil

    /// Activates the texture object
    public func activate(slot: Int?) {
        currentActiveSlot = slot
        if let slot = slot {
            glActiveTexture(GLint(GL_TEXTURE0) + GLint(slot))
        }
        glBindTexture(glTextureType, textureID)
    }

    /// Deactivates the texture object
    public func deactivate() {
        if let slot = currentActiveSlot {
            glActiveTexture(GLint(GL_TEXTURE0) + GLint(slot))
        }
        glBindTexture(glTextureType, 0)
    }

    /// Generate mipmaps for the texture.
    public func generateMipmaps() throws {
        active {
            glGenerateMipmap(glTextureType)
        }
    }

    /// The texture format
    public let format: TextureFormat

    /// The mapping from `TextureWrap` enum to OpenGL enum
    fileprivate static let textureWrapMapping: [TextureWrap: GLenum] = [
        .clampToEdge: GL_CLAMP_TO_EDGE,
        .clampToBorder: GL_CLAMP_TO_BORDER,
        .repeat: GL_REPEAT,
        .mirroredRepeat: GL_MIRRORED_REPEAT,
        .mirrorClampToEdge: GL_MIRROR_CLAMP_TO_EDGE
    ]

    /// Sets the texture border color. Used when the wrapping mode
    /// is set to `.clampToBorder`.
    public var borderColor: Color<Float> = Color<Float>() {
        didSet {
            let color: [Float] = [
                borderColor.red, borderColor.green, borderColor.blue,
                borderColor.alpha
            ]
            active {
                glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, color)
            }
        }
    }

    /// The mapping from `TextureMagnification` enum to OpenGL enum
    fileprivate static let textureMagnificationMapping: [TextureMagnification: GLenum] = [
        .nearest: GL_NEAREST,
        .linear: GL_LINEAR
    ]

    /// Sets the texture magnification and minification function is used
    /// whenever the level-of-detail function used when sampling from
    /// the texture determines that the texture should be magified or
    /// minified.
    public var magnification: TextureMagnification = .nearest {
        didSet {
            if let magnification = OpenGLTexture.textureMagnificationMapping[magnification] {
                active {
                    glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, magnification)
                }
            }
        }
    }

    /// The mapping from `TextureMinification` enum to OpenGL enum
    fileprivate static let textureMinificationMapping: [TextureMinification: GLenum] = [
        .nearest: GL_NEAREST,
        .linear: GL_LINEAR,
        .nearestMipmapNearest: GL_NEAREST_MIPMAP_NEAREST,
        .linearMipmapNearest: GL_LINEAR_MIPMAP_NEAREST,
        .nearestMipmapLinear: GL_NEAREST_MIPMAP_LINEAR,
        .linearMipmapLinear: GL_LINEAR_MIPMAP_LINEAR
    ]

    /// Sets the texture minifying function is used by the level-of-detail
    /// function when sampling from the texture determines that the
    /// texture should be minified.
    public var minification: TextureMinification = .nearest {
        didSet {
            if let minification = OpenGLTexture.textureMinificationMapping[minification] {
                active {
                    glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, minification)
                }
            }
        }
    }

    private var textureLevels: [TextureLevelIndex: TextureLevel] = [:]

    /// Returns a texture at the given mipmap level
    public subscript(level level: TextureLevelIndex) -> TextureLevel {
        if let level = self.textureLevels[level] {
            return level
        }
        self.textureLevels[level] = OpenGLTextureLevel(
                texture: self, level: level)
        return self.textureLevels[level]!
    }

}

public class OpenGLTexture1D: OpenGLTexture, Texture1D {

    /// Creates a new texture with the given format and size
    public required init(width: TextureDimension, format: TextureFormat) {
        self.width = TextureDimension(width)

        super.init(type: GL_TEXTURE_1D, format: format)

        // create a new texture object and load the image
        active {
            // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
//            glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT)

            // Set texture filtering parameters
            glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
            glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST)

            glTexImage1D(glTextureType, 0, format.glInternalFormat,
                    GLsizei(width), 0, format.glFormat, format.glType,
                    nil)
        }
    }

    /// The texture width
    public var width: TextureDimension {
        didSet {
            active {
                glTexImage1D(glTextureType, 0, format.glInternalFormat, GLsizei(width), 0, format.glFormat, format.glType, nil)
            }
        }
    }

    /// Sets the wrap mode for the texture
    public var wrap: TextureWrap = .repeat {
        didSet {
            if let s = OpenGLTexture.textureWrapMapping[wrap] {
                active {
                    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, s)
                }
            }
        }
    }

}

public class OpenGLTexture2D: OpenGLTexture, Texture2D {

    /// Creates a new texture with the given format and size
    public required init(width: TextureDimension, height: TextureDimension, format: TextureFormat) {
        self.width = TextureDimension(width)
        self.height = TextureDimension(height)

        super.init(type: GL_TEXTURE_2D, format: format)

        // create a new texture object and load the image
        active {

            // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
//        glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT)
//        glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT)

            // Set texture filtering parameters
            glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
            glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST)

            glTexImage2D(glTextureType, 0, format.glInternalFormat,
                    GLsizei(width), GLsizei(height),
                    0, format.glFormat, format.glType, nil)
        }
    }

    /// Creates a new texture object from the contents of a image
    public required init(image: Image) {
        width = TextureDimension(image.width)
        height = TextureDimension(image.height)

        super.init(
                type: GL_TEXTURE_2D,
                format: .color(precision: .uint8, channels: image.channels))

        active {
            // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
            glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT)
            glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT)

            // Set texture filtering parameters
            glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
            glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

            image.withUnsafeMutableBufferPointer {
                glTexImage2D(glTextureType, 0, format.glInternalFormat,
                        GLsizei(width),
                        GLsizei(height),
                        0, format.glFormat, format.glType,
                        $0.baseAddress)
            }
            try! self.generateMipmaps()
        }
    }

    /// The texture width
    public var width: TextureDimension {
        didSet {
            active {
                glTexImage2D(glTextureType, 0, format.glInternalFormat, GLsizei(width), GLsizei(height), 0, format.glFormat, format.glType, nil)
            }
        }
    }

    /// The texture height
    public var height: TextureDimension {
        didSet {
            active {
                glTexImage2D(glTextureType, 0, format.glInternalFormat, GLsizei(width), GLsizei(height), 0, format.glFormat, format.glType, nil)
            }
        }
    }

    /// Update a texture image data
    ///
    /// - Parameter image: the new image data to assign to the texture
    public func update(image: Image) {
        active {
            image.withUnsafeMutableBufferPointer {
                glTexImage2D(glTextureType, 0, format.glInternalFormat,
                        GLsizei(image.width),
                        GLsizei(image.height),
                        0, format.glFormat, format.glType,
                        $0.baseAddress)
            }
            width = TextureDimension(image.width)
            height = TextureDimension(image.height)
        }
    }

    /// Sets the wrap mode for the texture
    public var wrap: (s: TextureWrap, t: TextureWrap) = (.repeat, .repeat) {
        didSet {
            if let s = OpenGLTexture.textureWrapMapping[wrap.s],
               let t = OpenGLTexture.textureWrapMapping[wrap.t] {
                active {
                    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, s)
                    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, t)
                }
            }
        }
    }

}

public class OpenGLTexture3D: OpenGLTexture, Texture3D {

    /// Creates a new texture with the given format and size
    public required init(width: TextureDimension, height: TextureDimension, depth: TextureDimension, format: TextureFormat) {
        self.width = TextureDimension(width)
        self.height = TextureDimension(height)
        self.depth = TextureDimension(depth)

        super.init(type: GL_TEXTURE_3D, format: format)

        // create a new texture object and load the image
        active {
            // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
//        glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT)
//        glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT)

            // Set texture filtering parameters
            glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
            glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST)

            glTexImage3D(glTextureType, 0, format.glInternalFormat,
                    GLsizei(width), GLsizei(height), GLsizei(depth),
                    0, format.glFormat, format.glType, nil)
        }
    }

    /// The texture width
    public var width: TextureDimension {
        didSet {
            active {
                glTexImage3D(glTextureType, 0, format.glInternalFormat, GLsizei(width), GLsizei(height), GLsizei(depth),
                        0, format.glFormat, format.glType, nil)
            }
        }
    }

    /// The texture height
    public var height: TextureDimension {
        didSet {
            active {
                glTexImage3D(glTextureType, 0, format.glInternalFormat, GLsizei(width), GLsizei(height), GLsizei(depth),
                        0, format.glFormat, format.glType, nil)
            }
        }
    }

    /// The texture depth
    public var depth: TextureDimension {
        didSet {
            active {
                glTexImage3D(glTextureType, 0, format.glInternalFormat, GLsizei(width), GLsizei(height), GLsizei(depth),
                        0, format.glFormat, format.glType, nil)
            }
        }
    }

    /// Sets the wrap mode for the texture
    public var wrap: (r: TextureWrap, s: TextureWrap, t: TextureWrap) = (.repeat, .repeat, .repeat) {
        didSet {
            if let r = OpenGLTexture.textureWrapMapping[wrap.r],
               let s = OpenGLTexture.textureWrapMapping[wrap.s],
               let t = OpenGLTexture.textureWrapMapping[wrap.t] {
                active {
                    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_R, r)
                    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, s)
                    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, t)
                }
            }
        }
    }

    /// Returns a texture at the given mipmap level
    public subscript(layer layer: TextureLayerIndex,
                     level level: TextureLevelIndex) -> Texture3DLayer {
        if layer >= depth {
            fatalError("Could not create a Texture3DLayer because the texture has a depth of \(depth)")
        }
        return OpenGLTexture3DLayer(texture: self, level: level, layer: layer)
    }
}

public class OpenGLTexture3DLayer : OpenGLTextureLevel, Texture3DLayer {
    /// The 3D texture layer index
    public var layer: UInt

    internal init(texture: OpenGLTexture3D, level: TextureLevelIndex, layer: TextureLayerIndex) {
        self.layer = layer
        super.init(texture: texture, level: layer)
    }

    /// The texture width
    public var width: TextureDimension { return (self.texture as! OpenGLTexture3D).width }

    /// The texture height
    public var height: TextureDimension { return (self.texture as! OpenGLTexture3D).height }
}

public class OpenGLTextureCube: OpenGLTexture, TextureCube {

    /// Creates a new texture with the given format and size
    public required init(width: TextureDimension, height: TextureDimension, format: TextureFormat) {
        self.width = TextureDimension(width)
        self.height = TextureDimension(height)

        super.init(type: GL_TEXTURE_CUBE_MAP, format: format)

        active {
            // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
            glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT)
            glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT)
            glTexParameteri(glTextureType, GL_TEXTURE_WRAP_R, GL_REPEAT)

            // Set texture filtering parameters
            glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
            glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST)

            for i in 0..<6 {
                glTexImage2D(
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + GLenum(i),
                        0, format.glInternalFormat,
                        GLsizei(width), GLsizei(height),
                        0, format.glFormat, format.glType, nil)
            }
        }
    }

    /// Creates a new texture object from the contents of a image
    public required init(positiveX: Image, negativeX: Image,
                         positiveY: Image, negativeY: Image,
                         positiveZ: Image, negativeZ: Image) {
        width = TextureDimension(positiveX.width)
        height = TextureDimension(positiveX.height)

        super.init(
                type: GL_TEXTURE_CUBE_MAP,
                format: .color(precision: .uint8, channels: positiveX.channels))

        active {
            // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
            glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT)
            glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT)
            glTexParameteri(glTextureType, GL_TEXTURE_WRAP_R, GL_REPEAT)

            // Set texture filtering parameters
            glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
            glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

            for (i, image) in [positiveX, negativeX,
                               positiveY, negativeY,
                               positiveZ, negativeZ].enumerated() {
                image.withUnsafeMutableBufferPointer {
                    glTexImage2D(
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + GLenum(i),
                            0, format.glInternalFormat,
                            GLsizei(width),
                            GLsizei(height),
                            0, format.glFormat, format.glType,
                            $0.baseAddress)
                }
            }
            try! self.generateMipmaps()
        }
    }

    /// The texture width
    public var width: TextureDimension {
        didSet {
            active {
                for i in 0..<6 {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + GLenum(i),
                            0, format.glInternalFormat,
                            GLsizei(width),
                            GLsizei(height),
                            0, format.glFormat, format.glType,
                            nil)
                }
            }
        }
    }

    /// The texture height
    public var height: TextureDimension {
        didSet {
            active {
                for i in 0..<6 {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + GLenum(i),
                            0, format.glInternalFormat,
                            GLsizei(width),
                            GLsizei(height),
                            0, format.glFormat, format.glType,
                            nil)
                }
            }
        }
    }

    /// Sets the wrap mode for the texture
    public var wrap: (r: TextureWrap, s: TextureWrap, t: TextureWrap) = (.repeat, .repeat, .repeat) {
        didSet {
            if let r = OpenGLTexture.textureWrapMapping[wrap.r],
               let s = OpenGLTexture.textureWrapMapping[wrap.s],
               let t = OpenGLTexture.textureWrapMapping[wrap.t] {
                active {
                    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_R, r)
                    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, s)
                    glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, t)
                }
            }
        }
    }

    /// The the cube's positive-x face
    public lazy var positiveX: TextureCubeFace = OpenGLTextureCubeFace(
            parent: self, face: GL_TEXTURE_CUBE_MAP_POSITIVE_X)

    /// The the cube's negative-x face
    public lazy var negativeX: TextureCubeFace = OpenGLTextureCubeFace(
            parent: self, face: GL_TEXTURE_CUBE_MAP_NEGATIVE_X)

    /// The the cube's positive-y face
    public lazy var positiveY: TextureCubeFace = OpenGLTextureCubeFace(
            parent: self, face: GL_TEXTURE_CUBE_MAP_POSITIVE_Y)

    /// The the cube's negative-y face
    public lazy var negativeY: TextureCubeFace = OpenGLTextureCubeFace(
            parent: self, face: GL_TEXTURE_CUBE_MAP_NEGATIVE_Y)

    /// The the cube's positive-z face
    public lazy var positiveZ: TextureCubeFace = OpenGLTextureCubeFace(
            parent: self, face: GL_TEXTURE_CUBE_MAP_POSITIVE_Z)

    /// The the cube's negative-z face
    public lazy var negativeZ: TextureCubeFace = OpenGLTextureCubeFace(
            parent: self, face: GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)

    public class OpenGLTextureCubeFace: TextureCubeFace {
        internal let parent: OpenGLTextureCube
        internal let face: GLenum
        internal let level: GLuint

        init(parent: OpenGLTextureCube, face: GLenum, level: GLuint = 0) {
            self.parent = parent
            self.face = face
            self.level = level
        }

        /// The texture width
        public var width: TextureDimension {
            return parent.width
        }

        /// The texture height
        public var height: TextureDimension {
            return parent.height
        }

        /// Returns a texture at the given mipmap level
        public subscript(level level: TextureLevelIndex) -> TextureCubeFace {
            return OpenGLTextureCubeFace(parent: parent, face: face, level: level.glUInt)
        }
    }

}

public class OpenGLTextureLevel: TextureLevel {

    /// Create a new texture level
    public init(texture: OpenGLTexture, level: TextureLevelIndex) {
        self.texture = texture
        self.level = level
    }

    /// The parent texture
    public var texture: Texture

    /// The texture level
    public var level: TextureLevelIndex

}

internal extension TextureFormat {

    internal var glInternalFormat: GLint {
        switch self {
        case .color(.uint8,   1): return GL_R8
        case .color(.float16, 1): return GL_R16F
        case .color(.float32, 1): return GL_R32F

        case .color(.uint8,   2): return GL_RG8
        case .color(.float16, 2): return GL_RG16F
        case .color(.float32, 2): return GL_RG32F

        case .color(.uint8,   3): return GL_RGB8
        case .color(.float16, 3): return GL_RGB16F
        case .color(.float32, 3): return GL_RGB32F

        case .color(.uint8,   4): return GL_RGBA8
        case .color(.float16, 4): return GL_RGBA16F
        case .color(.float32, 4): return GL_RGBA32F

        case .depth(.uint8):   return GL_DEPTH_COMPONENT
        case .depth(.float16): return GL_DEPTH_COMPONENT16
        case .depth(.float32): return GL_DEPTH_COMPONENT32F
        default: fatalError("OpenGL does not support the given texture format: \(self)")
        }
    }

    internal var glFormat: GLenum {
        switch self {
        case .color(_, 1): return GL_R
        case .color(_, 2): return GL_RG
        case .color(_, 3): return GL_RGB
        case .color(_, 4): return GL_RGBA
        case .depth: return GL_DEPTH_COMPONENT
        default: fatalError("OpenGL does not support the given texture format: \(self)")
        }
    }

    internal var glType: GLenum {
        switch self {
        case .color(.uint8,   _), .depth(.uint8):   return UInt8.glType
        case .color(.float16, _), .depth(.float16): return GL_HALF_FLOAT
        case .color(.float32, _), .depth(.float32): return Float.glType
        }
    }

}