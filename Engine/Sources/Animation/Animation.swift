//
// Created by Rogiel Sulzbach on 10/21/17.
//

public class Animation<T> where T : Animatable {

    /// A closure that gets called for every animation progress
    public typealias AnimationBody = () -> Void

    /// The animation curve
    public var curve: AnimationCurve!

    public init(curve: AnimationCurve! = nil) {
        self.curve = curve
    }

    public func animate(_ body: AnimationBody) {

    }

}

public protocol Animatable {

}