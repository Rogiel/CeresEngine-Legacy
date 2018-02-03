//
//  mains.swift
//  Game
//
//  Created by Rogiel Sulzbach on 10/7/17.
//  Copyright © 2017 Rogiel Sulzbach. All rights reserved.
//

import Foundation
import Dispatch

import Game
import GameEngine
import GameEngineGLFW

import SGLOpenGL
import CGLFW3

ResourceManager.default.resourcesPath = Bundle.main.resourcePath!

let glfw = GLFW(width: 800, height: 600)!
let engine = GameEngine(
        renderer: OpenGLRenderer(),
        inputController: GameInputController()
)
(engine.inputController! as! GameInputController).engine = engine
engine.inputController?.mouses = [glfw.mouse]
engine.inputController?.keyboards = [glfw.keyboard]

let inputController = engine.inputController as! GameInputController
inputController.scene = engine.scene

print("Renderer up.")

func loadTexture(_ path: String) -> Texture2D? {
    guard let image = try? ResourceManager.default.load(named: path) as Image else {
        return nil
    }
    return engine.renderer.texture2D.init(image: image)
}

//func loadTextureAsync(_ path: String, completionHandler: @escaping (Texture2D?) -> Void) {
//    guard let resource = try? ResourceManager.default.resource(named: path) else {
//        return
//    }
//
//    resource.loadAsync { (image: Image?) in
//        if let image = image {
//            DispatchQueue.main.async {
//                completionHandler(engine.renderer.texture2D.init(image: image))
//            }
//        } else {
//            completionHandler(nil)
//        }
//    }
//}

func loadModel(named name: String) -> SimpleModel {
    print("Loading \(name)...")

    if name == "Earth" {
        let name2 = name
        var mesh = try! ResourceManager.default.load(named: "Planet/Earth/Earth.obj") as Mesh
        mesh.compile(renderer: engine.renderer)

        return SimpleModel(mesh: mesh, material: PBRMaterial(
                albedoMap: loadTexture("Planet/\(name2)/\(name2)_albedo.png"),
                normalMap: loadTexture("Planet/\(name2)/\(name2)_normal.png"),
                metallicMap: loadTexture("Planet/\(name2)/\(name2)_metallic.png"),
                roughnessMap: loadTexture("Planet/\(name2)/\(name2)_roughness.png"),
                ambientOcclusionMap: loadTexture("Planet/\(name2)/\(name2)_ao.png"),
                displacementMap: loadTexture("Planet/\(name2)/\(name2)_bump.png"),
                maximumDisplacement: 0.0 / 2.0
        ))
    } else {
        let name2 = name
        print("Planet/\(name)/\(name).obj")
        var mesh = try! ResourceManager.default.load(named: "Planet/\(name)/\(name).obj") as Mesh
        mesh.compile(renderer: engine.renderer)

        return SimpleModel(mesh: mesh, material: PBRMaterial(
                albedoMap: loadTexture("Planet/\(name2)/\(name2)_diffuse.png"),
                normalMap: loadTexture("Planet/\(name2)/\(name2)_normal.png"),
                metallicMap: loadTexture("Planet/\(name2)/\(name2)_specular.png"),
                roughnessMap: loadTexture("Planet/\(name2)/\(name2)_roughness.png"),
                ambientOcclusionMap: loadTexture("Planet/\(name2)/\(name2)_ao.png"),
                displacementMap: loadTexture("Planet/\(name2)/\(name2)_bump.png"),
                maximumDisplacement: 0.0 / 2.0
        ))
    }
}

glfwSwapInterval(0)

var bodies: [CelestialBody] = []

let sun = Star(
        name: "Sun",
        model: loadModel(named: "Sun"),
        mass: 1.989e33,
        radius: 696392000)
bodies.append(sun)
engine.entityManager.register(entity: sun)

let auToM = 149597870.700*1000.0
let epoch: Double = 946684800
let the10th: Double = 1518220800 - epoch

var earthOrbit = KeplerOrbit(
        primaryMass: sun.mass,
        secondaryMass: 5.9736e24,
        semiMajorAxis: 1.004751174396007E+00 * auToM,
        eccentricity: 1.591338849796446E-02,
        inclination: radians(8.869311254690260E-03),
        ascendingNode: radians(2.067704419359796E+02),
        argumentOfPeriapsis: radians(2.754279318663343E+02),
        trueAnomaly: radians(1.840052559047124E+01),
        at: the10th)
let earth = Planet(
        name: "Earth",
        star: sun,
        model: loadModel(named: "Earth"),
        orbit: earthOrbit,
        radius: 6371000,
        rotationPeriod: 24 * 3600)
engine.entityManager.register(entity: earth)
bodies.append(earth)

//let moon = Moon(
//        name: "Moon",
//        planet: earth,
//        model: loadModel(named: "Moon"),
//        orbit: KeplerOrbit(
//                primaryMass: earth.mass,
//                secondaryMass: 0.07346e24,
//                semiMajorAxis: 0.3844e9,
//                eccentricity: 0.0549,
//                inclination: radians(5.145),
//                ascendingNode: 0,
//                argumentOfPeriapsis: radians(6.68)),
//        radius: 1737.1e3,
//        rotationPeriod: 0)
//moon.rotationPeriod = moon.orbit.orbitalPeriod
//engine.entityManager.register(entity: moon)
//bodies.append(moon)

var roadsterOrbit = KeplerOrbit(
        primaryMass: sun.mass, secondaryMass: 1250.0,
        semiMajorAxis: 1.346782739324727E+00 * auToM,
        eccentricity: 2.653803281862795E-01,
        inclination: radians(1.110391008725130E+00),
        ascendingNode: radians(3.167970783953779E+02),
        argumentOfPeriapsis: radians(1.791663064550042E+02),
        trueAnomaly: radians(4.998779818046527E+00),
        at: the10th)
let roadster = Planet(
        name: "SpaceX Roadster",
        star: sun,
        model: loadModel(named: "ISS"),
        orbit: roadsterOrbit,
        radius: 108.5,
        rotationPeriod: 0)

roadster.rotationPeriod = roadster.orbit.orbitalPeriod
engine.entityManager.register(entity: roadster)
bodies.append(roadster)

//var issOrbit = KeplerOrbit(
//        primaryMass: sun.mass, secondaryMass: 1250.0,
//        semiMajorAxis: 1.346782739324727E+00 * auToM,
//        eccentricity: 2.653803281862795E-01,
//        inclination: radians(1.110391008725130E+00),
//        ascendingNode: radians(3.167970783953779E+02),
//        argumentOfPeriapsis: radians(1.791663064550042E+02),
//        trueAnomaly: radians(4.998779818046527E+00))
////issOrbit.trueAnomaly = issOrbit.trueAnomaly(from: 571678237.872914, to: 0)
//
//let iss = Planet(
//        name: "ISS",
//        star: sun,
//        model: loadModel(named: "ISS"),
////        orbit: KeplerOrbit(
////                primaryMass: earth.mass,
////                secondaryMass: 419455,
////                semiMajorAxis: 404.55e3 + earth.radius,
////                eccentricity: 0.0035,
////                inclination: radians(51.64),
////                ascendingNode: 0,
////                argumentOfPeriapsis: radians(6.68)),
//        orbit: issOrbit,
//        radius: 108.5,
//        rotationPeriod: 0)
//
//iss.rotationPeriod = iss.orbit.orbitalPeriod
//engine.entityManager.register(entity: iss)
//bodies.append(iss)

let mars = Planet(
        name: "Mars",
        star: sun,
        model: loadModel(named: "Mars"),
        orbit: KeplerOrbit(
                primaryMass: sun.mass,
                secondaryMass: 0.64171e24,

//EC= 9.826166270325440E-02 QR= 1.363260166184834E+00 IN= 1.847109315796484E+00
//OM= 4.927572833256983E+01 W = 2.886859150804260E+02 Tp=  2458378.449583463371
//N = 5.305761558968241E-01 MA= 2.438305716707213E+02 TA= 2.342890676690179E+02
//A = 1.511813471601585E+00 AD= 1.660366777018336E+00 PR= 6.785076864064839E+02
                semiMajorAxis: 1.511813471601585E+00 * auToM,
                eccentricity: 9.826166270325440E-02,
                inclination: radians(1.847109315796484E+00),
                ascendingNode: radians(4.927572833256983E+01),
                argumentOfPeriapsis: radians(2.886859150804260E+02),
                trueAnomaly: radians(2.342890676690179E+02)),
        radius: 3376.2e3,
        rotationPeriod: 3600 * 24.6229)
engine.entityManager.register(entity: mars)
bodies.append(mars)

let deimos = Moon(
        name: "Deimos",
        planet: mars,
        model: loadModel(named: "Deimos"),
        orbit: KeplerOrbit(
                primaryMass: mars.mass,
                secondaryMass: 1.4762e15,
                semiMajorAxis: 23463.2e3,
                eccentricity: 0.00033,
                inclination: radians(0.93),
                ascendingNode: 0,
                argumentOfPeriapsis: radians(6.68)),
        radius: 15.8837e3,
        rotationPeriod: 0
)
deimos.rotationPeriod = deimos.orbit.orbitalPeriod
engine.entityManager.register(entity: deimos)
bodies.append(deimos)

let phobos = Moon(
        name: "Phobos",
        planet: mars,
        model: loadModel(named: "Phobos"),
        orbit: KeplerOrbit(
                primaryMass: mars.mass,
                secondaryMass: 1.0659e16,
                semiMajorAxis: 9376e3,
                eccentricity: 0.0151,
                inclination: radians(1.093),
                ascendingNode: 0,
                argumentOfPeriapsis: radians(6.68)),
        radius: 15.8837e3,
        rotationPeriod: 0
)
phobos.rotationPeriod = phobos.orbit.orbitalPeriod
engine.entityManager.register(entity: phobos)
bodies.append(phobos)

let mercury = Planet(
        name: "Mercury",
        star: sun,
        model: loadModel(named: "Mercury"),
        orbit: KeplerOrbit(
                primaryMass: sun.mass,
                secondaryMass: 0.33011e24,
                semiMajorAxis: 57.91e10,
                eccentricity: 0.2056,
                inclination: radians(3.38),
                ascendingNode: radians(48.331),
                argumentOfPeriapsis: radians(29.124)),
        radius: 2439.7e3,
        rotationPeriod: 3600 * 24 * 58.646)
engine.entityManager.register(entity: mercury)
bodies.append(mercury)

let venus = Planet(
        name: "Venus",
        star: sun,
        model: loadModel(named: "Venus"),
        orbit: KeplerOrbit(
                primaryMass: sun.mass,
                secondaryMass: 4.8675e24,
                semiMajorAxis: 108.21e10,
                eccentricity: 0.0067,
                inclination: radians(3.86),
                ascendingNode: radians(76.680),
                argumentOfPeriapsis: radians(54.884)),
        radius: 6051.8e3,
        rotationPeriod: 3600 * 24 * 224.701)
engine.entityManager.register(entity: venus)
bodies.append(venus)

let jupiter = Planet(
        name: "Jupiter",
        star: sun,
        model: loadModel(named: "Jupiter"),
        orbit: KeplerOrbit(
                primaryMass: sun.mass,
                secondaryMass: 1898.19e24,
                semiMajorAxis: 778.57e10,
                eccentricity: 0.0489,
                inclination: radians(6.09),
                ascendingNode: radians(100.464),
                argumentOfPeriapsis: radians(273.867)),
        radius: 6051.8e3,
        rotationPeriod: 3600 * 9.925)
engine.entityManager.register(entity: jupiter)
bodies.append(jupiter)

inputController.bodies = bodies

engine.scene.camera = OrbitalCamera(object: earth)
engine.scene.camera.position = Vector3<Double>(1.0, 0.0, 0.0)
engine.scene.camera.yaw = 180
engine.scene.camera.nearPlane = 0.0001
engine.scene.camera.farPlane = earth.orbit!.semiMajorAxis * 10.0
engine.scene.camera.zoom = earth.radius * 20.0

let deferredRenderingTechnique = engine.renderingTechnique as! PBRDeferredRenderingTechnique
deferredRenderingTechnique.backgroundSkybox = engine.renderer.textureCube.init(
        positiveX: try! ResourceManager.default.load(named: "Skybox/Skybox_PositiveX.jpg"),
        negativeX: try! ResourceManager.default.load(named: "Skybox/Skybox_NegativeX.jpg"),
        positiveY: try! ResourceManager.default.load(named: "Skybox/Skybox_PositiveY.jpg"),
        negativeY: try! ResourceManager.default.load(named: "Skybox/Skybox_NegativeY.jpg"),
        positiveZ: try! ResourceManager.default.load(named: "Skybox/Skybox_PositiveZ.jpg"),
        negativeZ: try! ResourceManager.default.load(named: "Skybox/Skybox_NegativeZ.jpg")
)

glfwSetWindowSizeCallback(glfw.window) { (window, width, height) in
    deferredRenderingTechnique.resize(width: UInt(width), height: UInt(height))
    engine.scene.camera.aspectRatio = Double(width) / Double(height)
}

//let spotLight = SpotLight()
//spotLight.diffuse = Color<Float>(2.0)
//spotLight.specular = Color<Float>(2.0)
//spotLight.linear = 0.01
////spotLight.constant = 0.5
//spotLight.shadowOcclusionStrength = 0.9
//
//spotLight.cutOff = 20
//spotLight.outerCutOff = 25

//scene.lights.append(spotLight)

let worldLight = PointLight()
worldLight.diffuse = Color<Float>(382.8e22)
worldLight.specular = Color<Float>(1.0)
worldLight.ambient = Color<Float>(0.01)
worldLight.shadows = true
//worldLight.position = sun.position

worldLight.constant = 0.0
worldLight.linear = 1.0 / (earth.radius / 0.00000004)
//worldLight.linear = 0.0
//worldLight.quadratic = 0.0

engine.scene.lights.append(worldLight)

import CNanoVG
import GameEngineUI

let vg = nvgCreateGL3_swift(Int32(NVG_STENCIL_STROKES.rawValue | NVG_DEBUG.rawValue));

func drawButton(_ vg: OpaquePointer, _ text: String, _ x: Float, _ y: Float, _ w: Float, _ h: Float, _ col: NVGcolor) {
    let cornerRadius: Float = 4.0;
    var tw: Float = 0, iw: Float = 0;

    let bg = nvgLinearGradient(vg, x, y, x, y + h, nvgRGBA(255, 255, 255, 255 / 2), nvgRGBA(0, 0, 0, 255 / 2));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, cornerRadius - 1);
    nvgFillColor(vg, col);
    nvgFill(vg);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x + 0.5, y + 0.5, w - 1, h - 1, cornerRadius - 0.5);
    nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
    nvgStroke(vg);

    nvgFontSize(vg, 20.0);
    nvgFontFace(vg, "sans-bold");
    tw = nvgTextBounds(vg, 0, 0, text, nil, nil);

    nvgFontSize(vg, 20.0);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg, Int32(NVG_ALIGN_LEFT.rawValue | NVG_ALIGN_MIDDLE.rawValue));
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 160));
    nvgText(vg, x + w * 0.5 - tw * 0.5 + iw * 0.25, y + h * 0.5 - 1, text, nil);
    nvgFillColor(vg, nvgRGBA(255, 255, 255, 160));
    nvgText(vg, x + w * 0.5 - tw * 0.5 + iw * 0.25, y + h * 0.5, text, nil);
}

// UI
let label = UILabel()
label.frame.h = 15.0
label.frame.w = 0
label.frame.x = 10
label.frame.y = 10
label.textSize = 15.0
label.textColor = UIColor(red: 1, green: 1, blue: 1)

nvgCreateFont(vg, "sans", "/Users/Rogiel/Dropbox/Allogica/Outros/Fonts/Raleway-master/fonts/v3.000 Fontlab/TTF/Raleway-Regular.ttf");
nvgCreateFont(vg, "sans-bold", "/Users/Rogiel/Dropbox/Allogica/Outros/Fonts/Raleway-master/fonts/v3.000 Fontlab/TTF/Raleway-Bold.ttf");

// Game loop
var fpsSamples: [Double] = []

let dateFormatter = DateFormatter()
dateFormatter.dateStyle = .medium
dateFormatter.timeStyle = .medium

let formatter = NumberFormatter()
var deltaTime = Double(0.00)
while glfwWindowShouldClose(glfw.window) == 0 {
    if inputController.updateCamera(deltaTime: deltaTime) == false {
        glfw.close()
    }

    DispatchQueue.main.resume()

    // Check if any events have been activated
    // (key pressed, mouse moved etc.) and call
    // the corresponding response functions
    glfwPollEvents()

    deltaTime = engine.render(
            timeScale: inputController.timeScale
    )

    nvgBeginFrame(vg!,
            Int32(engine.renderer.defaultFramebuffer.width),
            Int32(engine.renderer.defaultFramebuffer.height),
            Float(engine.renderer.defaultFramebuffer.width) /
                    Float(engine.renderer.defaultFramebuffer.height))
    label.draw(context: vg!)
//    drawButton(vg!, "Hello", 200, 100, 28, 110, nvgRGBA(0, 0, 0, 28))
    nvgEndFrame(vg!)

    // Swap the screen buffers
    glfwSwapBuffers(glfw.window)

    fpsSamples.append(Double(1.0 / deltaTime))
    if fpsSamples.count > 60 {
        fpsSamples.removeSubrange(0...1)
    }
    let averageFPS = fpsSamples.reduce(0) {
        return $0 + $1
    } / Double(fpsSamples.count)

    let fps = formatter.string(from: NSNumber(floatLiteral: averageFPS))!
    label.text = "FPS: \(fps), Time Scale: \(inputController.timeScale)\nExposure: \(engine.scene.exposure)\nDate: \(dateFormatter.string(from: earth.date))"
//    glfwSetWindowTitle(glfw.window, "Rogiel Swift Game Engine - FPS: \(fps) - Time Scale: \(inputController.timeScale)")
}
