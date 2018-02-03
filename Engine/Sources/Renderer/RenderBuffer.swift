//
// Created by Rogiel Sulzbach on 10/28/17.
//

public protocol RenderBuffer : RenderTarget {
    /// Creates a new render buffer with the given parameters
    init(width: UInt, height: UInt, format: RenderBufferFormat)

    /// The render buffer format
    var format: RenderBufferFormat { get }

    /// The render buffer width
    var width: UInt { get set }

    /// The render buffer height
    var height: UInt { get set }
}

public enum RenderBufferFormat {
    /// A render buffer that has a color buffer with up to 4 color channels.
    case color(precision: RenderBufferPrecision, channels: Int)

    /// A render buffer that has a depth buffer attached to it.
    case depth(precision: RenderBufferPrecision)

    /// A render buffer that has stores a stencil buffer
    case stencil
}

/// The render buffer format precision
public enum RenderBufferPrecision {
    /// A render buffer that represents each channel using 8 bits
    case uint8

    /// A render buffer that represents each channel using 16 bit floats
    case float16

    /// A render buffer that represents each channel using 32 bit floats
    case float32
}