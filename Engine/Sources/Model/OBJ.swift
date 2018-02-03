//
//  OBJ.swift
//  GameEngine
//
//  Created by Rogiel Sulzbach on 10/6/17.
//

import Foundation
import CGameEngine

public class OBJMeshLoader : ResourceLoader {
    public func supports<T>(resource: Resource, ofType type: T.Type) -> Bool {
        return (type == Mesh.self || type == SimpleMesh.self)
                && resource.url.pathExtension == "obj"
    }

    /// Loads the resource
    public func load<T>(resource: Resource) throws -> T {
        guard let resource = resource as? FileResource else {
            throw ResourceLoadingError.invalidResourceType
        }
        print("Loading \(resource.url.path)")
        return parseOBJ(contentsOf: resource)! as! T
    }
}

fileprivate func parseOBJ(contentsOf resource: Resource) -> Mesh? {
    guard let resource = resource as? FileResource else {
        return nil
    }

#if false // if true, will use a pure Swift parser
    let source = try! String(contentsOf: resource.url)
    let parser = OBJParser(source: source.split(separator: "\n").map { String($0) })

    var vertices: [Vector3<Float>] = []
    parser.onVertex = { (x, y, z, w, r, g, b) in
        vertices.append(Vector3<Float>(x: Float(x), y: Float(y), z: Float(z)))
    }

    var texCoords: [Vector2<Float>] = []
    parser.onTextureCoord = { (u, v, w) in
        texCoords.append(Vector2<Float>(x: Float(u), y: Float(v)))
    }

    var normals: [Vector3<Float>] = []
    parser.onVertexNormal = { (x, y, z) in
        normals.append(Vector3<Float>(x: Float(x), y: Float(y), z: Float(z)))
    }

//    parser.onParameterSpaceVertex = { (u, v, w) in }
    var fullVertices: [Vertex] = []
    var indices: [Int] = []
    parser.onFace = { (count, vs, vtcs, vns) in
        if count != 3 {
            fatalError("Faces with \(count) vertices are not supported!")
        }

        for i in 0..<count {
            indices.append(indices.count)
            fullVertices.append(Vertex(
                position: vertices[vs[i] - 1],
                normal: normals[vns[i] - 1],
                texCoords: texCoords[vtcs[i] - 1],
                tangent: nil
            ))
        }
    }

    parser.parse()

#else

    let mesh = OBJLoad(resource.url.path)
    defer { OBJRelease(mesh) }
    
    var fullVertices: [Vertex] = []
    var indices: [UInt32] = []
    for i in 0..<mesh!.pointee.count {
        indices.append(UInt32(indices.count))
        fullVertices.append(Vertex(
            position: Vector3(mesh!.pointee.vertices[i].position.0, mesh!.pointee.vertices[i].position.1, mesh!.pointee.vertices[i].position.2),
            normal: Vector3(mesh!.pointee.vertices[i].normal.0, mesh!.pointee.vertices[i].normal.1, mesh!.pointee.vertices[i].normal.2),
            texCoords: Vector2(mesh!.pointee.vertices[i].texCoords.0, mesh!.pointee.vertices[i].texCoords.1),
            tangent: nil
        ))
    }

#endif

    var simpleMesh = SimpleMesh(vertices: fullVertices, indices: indices)
    simpleMesh.calculateTangent()

    return simpleMesh
}

public struct OBJParserStats {
    public var numberOfVertices: Int = 0
    public var numberOfTextureCoords: Int = 0
    public var numberOfVertexNormals: Int = 0
    public var numberOfParameterSpaceVertices: Int = 0
    public var numberOfFaces: Int = 0
}

public class OBJParser<T: Sequence> where T.Iterator.Element == String {
    public init(source: T) {
        self.source = source
        self.onVertex = { (x, y, z, w, r, g, b) in }
        self.onTextureCoord = { (u, v, w) in }
        self.onVertexNormal = { (x, y, z) in }
        self.onParameterSpaceVertex = { (u, v, w) in }
        self.onFace = { (count, vs, vtcs, vns) in }
        self.onUnknown = { (line) in }
    }
    
    public func count() -> OBJParserStats {
        var stats = OBJParserStats()
        for line in source {
            if line.hasPrefix("v ") {
                stats.numberOfVertices += 1
            } else if line.hasPrefix("vt ") {
                stats.numberOfTextureCoords += 1
            } else if line.hasPrefix("vn ") {
                stats.numberOfVertexNormals += 1
            } else if line.hasPrefix("vp ") {
                stats.numberOfParameterSpaceVertices += 1
            } else if line.hasPrefix("f ") {
                stats.numberOfFaces += 1
            }
        }
        return stats
    }
    
    public func parse() {
        var fVertices = [Int](), fTextureCoords = [Int](), fVertexNormals = [Int]()
        fVertices.reserveCapacity(4)
        fTextureCoords.reserveCapacity(4)
        fVertexNormals.reserveCapacity(4)
        
        for line in source {
            let scanner = Scanner(string: line)
            if line.hasPrefix("v ") {
                // # List of geometric vertices, with (x,y,z[,w]) coordinates, w is optional and defaults to 1.0.
                // also supports trailing r,g,b vertex colours
                // v 0.123 0.234 0.345 1.0
                scanner.scanLocation = 2
                var x: Double = 0.0,
                y: Double = 0.0,
                z: Double = 0.0,
                w: Double = 1.0,
                r: Double = -1.0,
                g: Double = -1.0,
                b: Double = -1.0
                scanner.scanDouble(&x)
                scanner.scanDouble(&y)
                scanner.scanDouble(&z)
                if !scanner.isAtEnd {
                    scanner.scanDouble(&w)
                    if !scanner.isAtEnd {
                        scanner.scanDouble(&r)
                        scanner.scanDouble(&g)
                        if !scanner.isAtEnd {
                            scanner.scanDouble(&b)
                        } else {
                            b = g
                            g = r
                            r = w
                            w = 1.0
                        }
                    }
                }
                onVertex(x, y, z, w, r, g, b)
            } else if line.hasPrefix("vt ") {
                // # List of texture coordinates, in (u, v [,w]) coordinates, these will vary between 0 and 1, w is optional and defaults to 0.
                // vt 0.500 1
                scanner.scanLocation = 3
                var u: Double = 0.0,
                v: Double = 0.0,
                w: Double = 0.0
                scanner.scanDouble(&u)
                scanner.scanDouble(&v)
                if !scanner.isAtEnd {
                    scanner.scanDouble(&w)
                }
                onTextureCoord(u, v, w)
            } else if line.hasPrefix("vn ") {
                // # List of vertex normals in (x,y,z) form; normals might not be unit vectors.
                // vn 0.707 0.000 0.707
                scanner.scanLocation = 3
                var x: Double = 0.0,
                y: Double = 0.0,
                z: Double = 0.0
                scanner.scanDouble(&x)
                scanner.scanDouble(&y)
                scanner.scanDouble(&z)
                onVertexNormal(x, y, z)
            } else if line.hasPrefix("vp ") {
                // # Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement
                // vp 0.310000 3.210000 2.100000
                scanner.scanLocation = 3
                var u: Double = 0.0,
                v: Double = 0.0,
                w: Double = 0.0
                scanner.scanDouble(&u)
                if !scanner.isAtEnd {
                    scanner.scanDouble(&v)
                    if !scanner.isAtEnd {
                        scanner.scanDouble(&w)
                    }
                }
                onParameterSpaceVertex(u, v, w)
            } else if line.hasPrefix("f ") {
                // # Polygonal face element
                // f 1 2 3
                // f 3/1 4/2 5/3
                // f 6/4/1 3/5/3 7/6/5
                // f 7//1 8//2 9//3
                scanner.scanLocation = 2
                var tmp: Int = 0, vertexCount: Int = 0
                while !scanner.isAtEnd {
                    scanner.scanInt(&tmp)
                    fVertices.append(tmp)
                    if getChar(scanner) == "/" {
                        scanner.scanLocation += 1
                        if getChar(scanner) != "/" {
                            scanner.scanInt(&tmp)
                            fTextureCoords.append(tmp)
                        } else {
                            fTextureCoords.append(-1)
                        }
                        if getChar(scanner) == "/" {
                            scanner.scanLocation += 1
                            scanner.scanInt(&tmp)
                            fVertexNormals.append(tmp)
                        } else {
                            fVertexNormals.append(-1)
                        }
                    } else {
                        fTextureCoords.append(-1)
                        fVertexNormals.append(-1)
                    }
                    vertexCount += 1
                }
                onFace(vertexCount, fVertices, fTextureCoords, fVertexNormals)
                fVertices.removeAll()
                fTextureCoords.removeAll()
                fVertexNormals.removeAll()
            } else if line.hasPrefix("#") {
                // comment, skip
            } else if !scanner.isAtEnd {
                onUnknown(line)
            }
        }
    }
    
    private func getChar(_ s: Scanner) -> Character {
        return s.string[s.string.index(s.string.startIndex, offsetBy: s.scanLocation)]
    }
    
    public var onVertex: (Double, Double, Double, Double, Double, Double, Double) -> Void
    public var onTextureCoord: (Double, Double, Double) -> Void
    public var onParameterSpaceVertex: (Double, Double, Double) -> Void
    public var onVertexNormal: (Double, Double, Double) -> Void
    public var onFace: (Int, [Int], [Int], [Int]) -> Void
    public var onUnknown: (String) -> Void
    
    private let source: T
}
