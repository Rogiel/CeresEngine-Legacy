//
//  Framebuffer.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

public protocol Framebuffer : class {

    /// Create a new framebuffer with the given size
    ///
    /// - Parameter width: the framebuffer width
    /// - Parameter height: the framebuffer height
    init(width: UInt, height: UInt)

    /// Create a new framebuffer with the size
    ///
    /// - Parameter width:              the framebuffer width
    /// - Parameter height:             the framebuffer height
    /// - Parameter colorAttachments:   the framebuffer color attachments
    /// - Parameter depthAttachment:    the framebuffer depth attachment
    init(width: UInt, height: UInt, colorAttachments: [RenderTarget],
         depthAttachment: RenderTarget?)

    /// Activates the framebuffer object
    func activate()

    /// Deactivates the framebuffer object
    func deactivate()

    /// Executes `body` within the context of the framebuffer.
    func active(body: () throws -> Void) rethrows

    // MARK: - Operations

    /// Copy the contents of the framebuffer to the <tt>destination</tt>
    /// framebuffer.
    ///
    /// - Parameter destination: the destination framebuffer
    func copy(destination: Framebuffer)

    /// Copy the depth contents of the framebuffer to the <tt>destination</tt>
    /// framebuffer.
    ///
    /// - Parameter destination: the destination framebuffer
    func copyDepth(destination: Framebuffer)

    /// Clear the framebuffer contents
    ///
    /// - Parameter color: the color to set the buffer to
    func clear(color: Color<Float>?)

    // MARK: - Input & Outputs

    /// The render target outputs for this framebuffer
    var colorAttachments: [RenderTarget] { get set }

    /// The render target for the depth buffer
    var depthAttachment: RenderTarget? { get set }

    /// The render target for the stencil buffer
    var stencilAttachment: RenderTarget? { get set }

    // MARK: - Options

    /// The framebuffer width
    var width: UInt { get set }

    /// The framebuffer height
    var height: UInt { get set }

    /// Enables or disables the blending mode on the framebuffer
    var blending: Bool { get set }

    /// Sets the framebuffer blending mode
    var blendingMode: FramebufferBlendingMode { get set }

    /// Enables or disables the depth testing on the framebuffer
    var depthTest: Bool { get set }

    /// Sets the framebuffer depth testing mode
    var depthTestingMode: FramebufferDepthTestingMode { get set }

    /// Enables or disables the face culling on the framebuffer
    var faceCulling: Bool { get set }

}

public extension Framebuffer {

    /// Create a new framebuffer with the size
    ///
    /// - Parameter width:              the framebuffer width
    /// - Parameter height:             the framebuffer height
    /// - Parameter colorAttachments:   the framebuffer color attachments
    public init(width: UInt, height: UInt, colorAttachments: [RenderTarget]) {
        self.init(width: width, height: height, colorAttachments: colorAttachments, depthAttachment: nil)
    }

    /// Create a new framebuffer with the size
    ///
    /// - Parameter width:              the framebuffer width
    /// - Parameter height:             the framebuffer height
    /// - Parameter depthAttachment:    the framebuffer depth attachment
    public init(width: UInt, height: UInt, depthAttachment: RenderTarget?) {
        self.init(width: width, height: height, colorAttachments: [], depthAttachment: depthAttachment)
    }

    /// Executes `body` within the context of the framebuffer.
    public func active(body: () throws -> Void) rethrows {
        activate()
        try body()
        deactivate()
    }

}

public protocol RenderTarget : class {

}

public enum FramebufferBlendingMode {

    /// Disables blending
    case none

    /// A mode that blends two
    case mix(source: Float, destination: Float)

}

public enum FramebufferDepthTestingMode {
    /// Passes if the incoming depth value is less than the stored depth value.
    case less

    /// Passes if the incoming depth value is less than or equal to the stored depth value.
    case lessThanOrEqual

    /// Passes if the incoming depth value is greater than the stored depth value.
    case greater

    /// Passes if the incoming depth value is greater than or equal to the stored depth value.
    case greaterThanOrEqual

    /// Passes if the incoming depth value is equal to the stored depth value.
    case equal

    /// Passes if the incoming depth value is not equal to the stored depth value.
    case notEqual

    /// Never passes.
    case never

    /// Always passes.
    case always
}

