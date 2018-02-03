//
// Created by Rogiel Sulzbach on 10/26/17.
//

import GameEngine

public class GameInputController: InputController {

    public var engine: GameEngine!
    public var scene: Scene!
    public var zoomScaler: Double = 0.0

    public override func mouse(_ mouse: Mouse, scrollBy scrollOffset: Double, direction: MouseScrollDirection) {
        if direction == .vertical {
            scene.camera.zoom += scrollOffset * zoomScaler * (keyboards.first!.pressed(key: .leftControl) ? 10.0 : 1.0) /* * parameters.R / 500.0*/
        }
    }

    private var lastPosition: Mouse.Position!

    public override func mouse(_ mouse: Mouse, didMoveToLocation location: Mouse.Position) {
        if mouse.isRightButtonPressed == false {
            lastPosition = nil
            return
        }

        if lastPosition == nil {
            lastPosition = location
        }

        let sensitivity = 0.05  // Change this value to your liking
        let offset = (lastPosition - location) * Mouse.Position(
                x: -1.0 * sensitivity, // X axis is inverted
                y: +1.0 * sensitivity
        )

        var yaw = scene.camera.yaw
        var pitch = scene.camera.pitch

        yaw += offset.x
        pitch += offset.y

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0) {
            pitch = 89.0
        }
        if (pitch < -89.0) {
            pitch = -89.0
        }

        scene.camera.yaw = yaw
        scene.camera.pitch = pitch

        lastPosition = location
    }

    private var currentBodyIndex: Int = 0
    public var bodies: [CelestialBody] = []
    public var timeScale: Double = 1.0

    public override func keyboard(_ keyboard: Keyboard, releaseKey key: KeyboardKey, modifiers: KeyboardModifier) {
        switch (key, modifiers) {
        case (.keypadAdd, [.shift]):
            timeScale *= 2.0
        case (.keypadAdd, []):
            timeScale *= 10.0
        case (.keypadSubtract, [.shift]):
            timeScale /= 2.0
        case (.keypadSubtract, []):
            timeScale /= 10.0

        case (.f1, []):
            if scene.exposure > 0.1 {
                scene.exposure -= 0.1
            }
        case (.f2, []):
            scene.exposure += 0.1

        case (.f3, []):
            if let deferredRenderingTechnique = engine?.renderingTechnique as? DeferredRenderingTechnique {
                if deferredRenderingTechnique.bloomAmount > 0 {
                    deferredRenderingTechnique.bloomAmount -= 1
                }
            } else if let deferredRenderingTechnique = engine?.renderingTechnique as? PBRDeferredRenderingTechnique {
                if deferredRenderingTechnique.bloomAmount > 0 {
                    deferredRenderingTechnique.bloomAmount -= 1
                }
            }

        case (.f4, []):
            if let deferredRenderingTechnique = engine?.renderingTechnique as? DeferredRenderingTechnique {
                deferredRenderingTechnique.bloomAmount += 1
            } else if let deferredRenderingTechnique = engine?.renderingTechnique as? PBRDeferredRenderingTechnique {
                deferredRenderingTechnique.bloomAmount += 1
            }

//        case (.f5, []):
//            if renderingTechnique === deferredRenderingTechnique {
//                renderingTechnique = forwardRenderingTechnique
//            } else {
//                renderingTechnique = deferredRenderingTechnique
//            }

        case (.f6, []):
            if let deferredRenderingTechnique = engine?.renderingTechnique as? DeferredRenderingTechnique {
                deferredRenderingTechnique.bloom = !deferredRenderingTechnique.bloom
            } else if let deferredRenderingTechnique = engine?.renderingTechnique as? PBRDeferredRenderingTechnique {
                deferredRenderingTechnique.bloom = !deferredRenderingTechnique.bloom
            }
        case (.f7, []):
            if let deferredRenderingTechnique = engine?.renderingTechnique as? DeferredRenderingTechnique {
                deferredRenderingTechnique.hdr = !deferredRenderingTechnique.hdr
            } else if let deferredRenderingTechnique = engine?.renderingTechnique as? PBRDeferredRenderingTechnique {
                deferredRenderingTechnique.hdr = !deferredRenderingTechnique.hdr
            }
        case (.f8, []):
            if let deferredRenderingTechnique = engine?.renderingTechnique as? DeferredRenderingTechnique {
                deferredRenderingTechnique.shadows = !deferredRenderingTechnique.shadows
            } else if let deferredRenderingTechnique = engine?.renderingTechnique as? PBRDeferredRenderingTechnique {
                deferredRenderingTechnique.shadows = !deferredRenderingTechnique.shadows
            }
        case (.f12, []):
            if let deferredRenderingTechnique = engine?.renderingTechnique as? DeferredRenderingTechnique {
                deferredRenderingTechnique.isDebugViewEnabled = !deferredRenderingTechnique.isDebugViewEnabled
            } else if let deferredRenderingTechnique = engine?.renderingTechnique as? PBRDeferredRenderingTechnique {
                deferredRenderingTechnique.isDebugViewEnabled = !deferredRenderingTechnique.isDebugViewEnabled
            }
        case (.rightBracket, []):
            currentBodyIndex += 1
            if currentBodyIndex > bodies.count - 1 {
                currentBodyIndex = 0
            }
            if let camera = scene.camera as? OrbitalCamera {
                camera.object = bodies[currentBodyIndex]
                camera.zoom = bodies[currentBodyIndex].radius * 3.0
                zoomScaler = bodies[currentBodyIndex].radius / 10.0
            }
        case (.leftBracket, []):
            currentBodyIndex -= 1
            if currentBodyIndex < 0 {
                currentBodyIndex = bodies.count - 1
            }
            if let camera = scene.camera as? OrbitalCamera {
                camera.object = bodies[currentBodyIndex]
                camera.zoom = bodies[currentBodyIndex].radius * 3.0
                zoomScaler = bodies[currentBodyIndex].radius / 10.0
            }

        default: return
        }
    }

    public func updateCamera(deltaTime: Double) -> Bool {
        // Camera controls
        let cameraSpeed = 5.0 * deltaTime
        if keyboards.first!.pressed(key: .w) {
            scene.camera.position += cameraSpeed * scene.camera.front
        }
        if keyboards.first!.pressed(key: .s) {
            scene.camera.position -= cameraSpeed * scene.camera.front
        }
        if keyboards.first!.pressed(key: .a) {
            scene.camera.position -= normalize(cross(scene.camera.front, scene.camera.up)) * cameraSpeed
        }
        if keyboards.first!.pressed(key: .d) {
            scene.camera.position += normalize(cross(scene.camera.front, scene.camera.up)) * cameraSpeed
        }

        return !keyboards.first!.pressed(key: .escape)
    }
}
