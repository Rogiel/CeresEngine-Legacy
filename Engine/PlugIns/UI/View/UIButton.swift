//
// Created by Rogiel Sulzbach on 10/28/17.
//

import Foundation
import CNanoVG

public class UIButton: UIControl {

    public var backgroundColor: UIColor = UIButtonTheme.backgroundColor
    public var borderColor: UIColor = UIButtonTheme.borderColor
    public var borderRadius: Double = UIButtonTheme.borderRadius

    public var text: String! = nil
    public var textSize: Double = 20.0
    public var textColor: UIColor = UIButtonTheme.textColor
    public var textShadow: Bool = true
    public var textShadowColor: UIColor = UIButtonTheme.textColor

    public override func draw(context: DrawContext) {
        let frame = self.frame

        UIPath.draw(context) {
            $0.rect(origin: UIPoint(x: frame.x + 1, y: frame.y + 1),
                            size: UIPoint(x: frame.w - 2, y: frame.h - 2),
                            borderRadius: self.borderRadius - 1)
                    .fill(color: self.backgroundColor)
        }

        UIPath.draw(context) {
            $0.rect(origin: UIPoint(x: frame.x + 0.5, y: frame.y + 0.5),
                            size: UIPoint(x: frame.w - 1, y: frame.h - 1),
                            borderRadius: self.borderRadius - 0.5)
                    .stroke(color: self.borderColor)
        }

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
            nvgText(context, Float(frame.x + frame.w * 0.5 - tw * 0.5), Float(frame.y + frame.h * 0.5 - 1), text, nil)
        }

//        NVGpaint bg;
//        char icon[8];
//        float cornerRadius = 4.0f;
//        float tw = 0, iw = 0;
//
//        bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(255,255,255,isBlack(col)?16:32), nvgRGBA(0,0,0,isBlack(col)?16:32));
//        nvgBeginPath(vg);
//        nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);
//        if (!isBlack(col)) {
//            nvgFillColor(vg, col);
//            nvgFill(vg);
//        }
//        nvgFillPaint(vg, bg);
//        nvgFill(vg);
//
//        nvgBeginPath(vg);
//        nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
//        nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
//        nvgStroke(vg);
//
//        nvgFontSize(vg, 20.0f);
//        nvgFontFace(vg, "sans-bold");
//        tw = nvgTextBounds(vg, 0,0, text, NULL, NULL);
//        if (preicon != 0) {
//            nvgFontSize(vg, h*1.3f);
//            nvgFontFace(vg, "icons");
//            iw = nvgTextBounds(vg, 0,0, cpToUTF8(preicon,icon), NULL, NULL);
//            iw += h*0.15f;
//        }
//
//        if (preicon != 0) {
//            nvgFontSize(vg, h*1.3f);
//            nvgFontFace(vg, "icons");
//            nvgFillColor(vg, nvgRGBA(255,255,255,96));
//            nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
//            nvgText(vg, x+w*0.5f-tw*0.5f-iw*0.75f, y+h*0.5f, cpToUTF8(preicon,icon), NULL);
//        }
//
//        nvgFontSize(vg, 20.0f);
//        nvgFontFace(vg, "sans-bold");
//        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
//        nvgFillColor(vg, nvgRGBA(0,0,0,160));
//        nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f-1,text, NULL);
//        nvgFillColor(vg, nvgRGBA(255,255,255,160));
//        nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f,text, NULL);

    }

}

public protocol UIButtonDelegate {

    func button(_ button: UIButton)

}

public class UIButtonTheme {

    public static var backgroundColor = UIColor(red: 1, green: 1, blue: 1, alpha: 0.1)
    public static var borderColor = UIColor(red: 0, green: 0, blue: 0)
    public static var borderRadius: Double = 10.0

    public static var textColor = UIColor(red: 0, green: 0, blue: 0)

}