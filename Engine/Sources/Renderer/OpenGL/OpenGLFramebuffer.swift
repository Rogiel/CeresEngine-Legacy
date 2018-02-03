//
//  OpenGLFramebuffer.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

import SGLOpenGL

/// A type that identifies a OpenGL Framebuffer object
public typealias OpenGLFramebufferID = GLuint

/// Framebuffer Objects are OpenGL Objects, which allow for the creation of
/// user-defined Framebuffers. With them, one can render to non-Default
/// Framebuffer locations, and thus render without disturbing the main screen.
public class OpenGLFramebuffer: Framebuffer {

    /// The OpenGL framebuffer object ID
    public let framebufferID: OpenGLFramebufferID

    /// Create a new framebuffer with the given size
    ///
    /// - Parameter width: the framebuffer width
    /// - Parameter height: the framebuffer height
    public required init(width: UInt, height: UInt) {
        self.framebufferID = OpenGLFramebuffer.allocate()
        self.width = width
        self.height = height
    }

    /// Create a new framebuffer with the size
    ///
    /// - Parameter width:              the framebuffer width
    /// - Parameter height:             the framebuffer height
    /// - Parameter colorAttachments:   the framebuffer color attachments
    /// - Parameter depthAttachment:    the framebuffer depth attachment
    public convenience required init(width: UInt, height: UInt, colorAttachments: [RenderTarget],
                                     depthAttachment: RenderTarget?) {
        self.init(width: width, height: height)
        self.colorAttachments = colorAttachments
        self.depthAttachment = depthAttachment
        self.updateAttachments()
    }

    /// Creates a new OpenGL Framebuffer object from a already existing
    /// framebuffer. The framebuffer size will be derived from the current
    /// viewport size.
    public init(framebufferID: OpenGLFramebufferID) {
        self.framebufferID = framebufferID

        glBindFramebuffer(target: GL_FRAMEBUFFER, framebuffer: framebufferID)
//        var vp: [GLint] = [0, 0, 0, 0]
        var vp: (x0: GLint, y0: GLint, x1: GLint, y1: GLint) = (0, 0, 0, 0)
        glGetIntegerv(GL_VIEWPORT, &vp.x0)
        glBindFramebuffer(target: GL_FRAMEBUFFER, framebuffer: 0)

        self.width = UInt(vp.x1)
        self.height = UInt(vp.y1)
    }

    deinit {
        var fbo = framebufferID
        glDeleteFramebuffers(1, &fbo)
    }

    private static func allocate() -> OpenGLFramebufferID {
        var id: OpenGLFramebufferID = 0
        glGenFramebuffers(1, &id)
        return id
    }

    private func enableOrDisable(option: GLenum, state: Bool) {
        if state {
            glEnable(option)
        } else {
            glDisable(option)
        }
    }

    private var isActive: Bool = false
    private var activationStack: Int = 0

    /// Activates the framebuffer object
    public func activate() {
        activationStack += 1
        if isActive {
            return
        }

        glBindFramebuffer(target: GL_FRAMEBUFFER, framebuffer: framebufferID)
        glViewport(0, 0, GLsizei(width), GLsizei(height))

        enableOrDisable(option: GL_CULL_FACE, state: faceCulling)
        enableOrDisable(option: GL_DEPTH_TEST, state: depthTest)
        enableOrDisable(option: GL_BLEND, state: blending)

        switch blendingMode {
        case .none: enableOrDisable(option: GL_BLEND, state: false);
        case .mix:
            glBlendFunc(GL_ONE, GL_ONE)
        }

        if depthTest {
            switch depthTestingMode {
            case .never:             glDepthFunc(GL_NEVER)
            case .less:              glDepthFunc(GL_LESS)
            case .lessThanOrEqual:   glDepthFunc(GL_LEQUAL)
            case .greater:           glDepthFunc(GL_GREATER)
            case .greaterThanOrEqual:glDepthFunc(GL_GEQUAL)
            case .equal:             glDepthFunc(GL_EQUAL)
            case .notEqual:          glDepthFunc(GL_NOTEQUAL)
            case .always:            glDepthFunc(GL_ALWAYS)
            }
        }

        isActive = true
    }

    /// Deactivates the framebuffer object
    public func deactivate() {
        activationStack -= 1

        if activationStack == 0 {
            glBindFramebuffer(target: GL_FRAMEBUFFER, framebuffer: 0)
            isActive = false
        }
    }

    /// Copy the contents of the framebuffer to the <tt>destination</tt>
    /// framebuffer.
    ///
    /// - Parameter destination: the destination framebuffer
    public func copy(destination: Framebuffer) {
        guard let destination = destination as? OpenGLFramebuffer else {
            return
        }
        active {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination.framebufferID);
            glBlitFramebuffer(
                    0, 0, GLint(width), GLint(height),
                    0, 0, GLint(destination.width), GLint(destination.height),
                    GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST
            );
        }
    }

    /// Copy the depth contents of the framebuffer to the <tt>destination</tt>
    /// framebuffer.
    ///
    /// - Parameter destination: the destination framebuffer
    public func copyDepth(destination: Framebuffer) {
        guard let destination = destination as? OpenGLFramebuffer else {
            fatalError("Destination Framebuffer must be a OpenGL framebuffer!")
        }

        precondition(width == destination.width)
        precondition(height == destination.height)

        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination.framebufferID);
        glBlitFramebuffer(
                0, 0, GLint(width), GLint(height),
                0, 0, GLint(destination.width), GLint(destination.height),
                GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /// Clear the framebuffer contents
    ///
    /// - Parameter color: the color to set the buffer to
    public func clear(color: Color<Float>?) {
        active {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if let color = color {
                glClearColor(color.red, color.green, color.blue, color.alpha);
            } else {
                glClearColor(0.0, 0.0, 0.0, 1.0);
            }
        }
    }

    // MARK: - Input & Outputs

    /// The render target outputs for this framebuffer
    public var colorAttachments: [RenderTarget] = [] {
        didSet {
            updateAttachments()
        }
    }

    /// The render target for the depth buffer
    public var depthAttachment: RenderTarget? = nil {
        didSet {
            updateAttachments()
        }
    }

    /// The render target for the stencil buffer
    public var stencilAttachment: RenderTarget? = nil {
        didSet {
            updateAttachments()
        }
    }

    /// Update the OpenGL framebuffer attachments
    private func updateAttachments() {
        active {
            if colorAttachments.isEmpty {
                glDrawBuffer(GL_NONE)
            } else {
                var buffers: [GLenum] = []
                for (i, attachment) in colorAttachments.enumerated() {
                    attachment.attach(GL_COLOR_ATTACHMENT0 + GLint(i))
                    buffers.append(GL_COLOR_ATTACHMENT0 + GLenum(i))
                }
                glDrawBuffers(GLsizei(buffers.count), buffers)
            }
            glReadBuffer(GL_NONE)

            depthAttachment?.attach(GL_DEPTH_ATTACHMENT)
            stencilAttachment?.attach(GL_STENCIL_ATTACHMENT)
        }
    }

    // MARK: - Options

    /// The framebuffer width
    public var width: UInt

    /// The framebuffer height
    public var height: UInt

    /// Enables or disables the blending mode on the framebuffer
    public var blending: Bool = false

    /// Sets the framebuffer blending mode
    public var blendingMode: FramebufferBlendingMode = .none

    /// Enables or disables the depth testing on the framebuffer
    public var depthTest: Bool = true

    /// Sets the framebuffer depth testing mode
    public var depthTestingMode: FramebufferDepthTestingMode = .less

    /// Enables or disables the face culling on the framebuffer
    public var faceCulling: Bool = false

}

fileprivate extension RenderTarget {

    func attach(_ slot: GLenum) {
        /// This is ordered by most common as a small optimization!
        if let attachment = self as? OpenGLTexture2D {
            glFramebufferTexture2D(GL_FRAMEBUFFER, slot, GL_TEXTURE_2D, attachment.textureID, 0)
        } else if let attachment = self as? OpenGLRenderBuffer {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, slot, GL_RENDERBUFFER, attachment.renderBufferID)
        } else if let attachment = self as? OpenGLTextureCube {
            for face in 0..<6 {
                glFramebufferTexture2D(GL_FRAMEBUFFER, slot, GL_TEXTURE_CUBE_MAP_POSITIVE_X + GLenum(face),
                        attachment.textureID, 0)
            }
        } else if let attachment = self as? OpenGLTextureCube.OpenGLTextureCubeFace {
            glFramebufferTexture2D(GL_FRAMEBUFFER, slot, attachment.face, attachment.parent.textureID, GLint(attachment.level))
        } else if let attachment = self as? OpenGLTexture3DLayer {
            // this must be before OpenGLTextureLevel!
            attachment.texture.attach(slot, level: attachment.level, layer: attachment.layer)
        } else if let attachment = self as? OpenGLTextureLevel {
            // forward to the parent texture with the given level
            attachment.texture.attach(slot, level: attachment.level)
        } else if let attachment = self as? OpenGLTexture1D {
            glFramebufferTexture1D(GL_FRAMEBUFFER, slot, GL_TEXTURE_1D, attachment.textureID, 0)
        } else {
            fatalError("OpenGL does not support a render target of type \(type(of: self))")
        }
    }

    func attach(_ slot: GLenum, level: UInt) {
        /// This is ordered by most common as a small optimization!
        if let attachment = self as? OpenGLTexture2D {
            glFramebufferTexture2D(GL_FRAMEBUFFER, slot, GL_TEXTURE_2D, attachment.textureID, GLint(level))
        } else if let attachment = self as? OpenGLTextureCube.OpenGLTextureCubeFace {
            glFramebufferTexture2D(GL_FRAMEBUFFER, slot, attachment.face, attachment.parent.textureID, GLint(level))
        } else if let attachment = self as? OpenGLTexture1D {
            glFramebufferTexture1D(GL_FRAMEBUFFER, slot, GL_TEXTURE_1D, attachment.textureID, GLint(level))
        } else {
            fatalError("OpenGL does not support a render target level \(level) of type \(type(of: self))")
        }
    }

    func attach(_ slot: GLenum, level: UInt, layer: UInt) {
        /// This is ordered by most common as a small optimization!
        if let attachment = self as? OpenGLTexture3D {
            glFramebufferTexture3D(GL_FRAMEBUFFER, slot, GL_TEXTURE_3D, attachment.textureID, GLint(level), GLint(layer))
        } else {
            fatalError("OpenGL does not support a render target level \(level) at layer \(layer) of type \(type(of: self))")
        }
    }

}
