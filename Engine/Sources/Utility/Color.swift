//
//  Color.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/3/17.
//

public struct Color<T : Numeric> {
    
    /// The red color channel
    public let red: T
    
    /// The green color channel
    public let green: T
    
    /// The blue color channel
    public let blue: T
    
    /// The alpha channel (optional)
    public let alpha: T
    
    /// Creates a empty color.
    public init() {
        self.red = 1
        self.green = 1
        self.blue = 1
        self.alpha = 1
    }
    
    /// Create a new color from the RGBA values
    public init(_ color: T, alpha: T? = nil) {
        self.red = color
        self.green = color
        self.blue = color
        self.alpha = alpha ?? 1
    }
    
    /// Create a new color from the RGBA values
    public init(red: T, green: T, blue: T, alpha: T? = nil) {
        self.red = red
        self.green = green
        self.blue = blue
        self.alpha = alpha ?? 1
    }

}

