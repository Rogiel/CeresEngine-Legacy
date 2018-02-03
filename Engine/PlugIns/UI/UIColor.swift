//
// Created by Rogiel Sulzbach on 10/28/17.
//

import Foundation

public class UIColor {

    public var red: Double;
    public var green: Double;
    public var blue: Double;
    public var alpha: Double;

    public init(red: Double,
                green: Double,
                blue: Double,
                alpha: Double = 1) {
        self.red = red
        self.green = green
        self.blue = blue
        self.alpha = alpha
    }

    public convenience init() {
        self.init(red: 0, green: 0, blue: 0)
    }

}
