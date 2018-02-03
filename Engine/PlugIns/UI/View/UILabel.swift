//
// Created by Rogiel Sulzbach on 10/28/17.
//

import Foundation
import CNanoVG

public class UILabel : UIView {

    public var text: String! = nil
    public var textSize: Double = 20.0
    public var textColor: UIColor = UIButtonTheme.textColor
    public var textShadow: Bool = true
    public var textShadowColor: UIColor = UIButtonTheme.textColor

    public override func draw(context: DrawContext) {
        let frame = self.frame

        if let text = self.text {
            nvgFontSize(context, Float(self.textSize))
            nvgFontFace(context, "sans")
            let tw = Double(nvgTextBounds(context, 0, 0, text, nil, nil))

            nvgFontSize(context, Float(self.textSize))
            nvgFontFace(context, "sans")
            nvgTextAlign(context, Int32(NVG_ALIGN_LEFT.rawValue | NVG_ALIGN_MIDDLE.rawValue))

            if self.textShadow {
                nvgFillColor(context, nvgRGBA(
                        UInt8(255 * textShadowColor.red),
                        UInt8(255 * textShadowColor.green),
                        UInt8(255 * textShadowColor.blue),
                        UInt8(255 * textShadowColor.alpha)
                ))
                nvgText(context, Float(frame.x + frame.w * 0.5 - tw * 0.5), Float(frame.y + frame.h * 0.5 - 0), text, nil)
            }

            nvgFillColor(context, nvgRGBA(
                    UInt8(255 * self.textColor.red),
                    UInt8(255 * self.textColor.green),
                    UInt8(255 * self.textColor.blue),
                    UInt8(255 * self.textColor.alpha)
            ))
            nvgText(context, Float(frame.x + frame.w * 0.5), Float(frame.y + frame.h * 0.5 - 1), text, nil)
        }
    }

}
