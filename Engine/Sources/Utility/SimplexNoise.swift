//
// Created by Rogiel Sulzbach on 10/22/17.
//

import Foundation

public class SimplexNoise {

    /// Frequency ("width") of the first octave of noise (default to 1.0)
    public var frequency: Double

    /// Amplitude ("height") of the first octave of noise (default to 1.0)
    public var amplitude: Double

    /// Lacunarity specifies the frequency multiplier between successive octaves (default to 2.0)
    public var lacunarity: Double

    /// Persistence is the loss of amplitude between successive octaves (usually 1/lacunarity)
    public var persistence: Double

    /// Creates a new simplex noise generator
    public init(frequency: Double = 1.0,
                amplitude: Double = 1.0,
                lacunarity: Double = 2.0,
                persistence: Double? = nil) {
        self.frequency = frequency
        self.amplitude = amplitude
        self.lacunarity = lacunarity
        self.persistence = persistence ?? 1 / lacunarity
    }

    /// Fractal/Fractional Brownian Motion (fBm) summation of 1D Perlin Simplex noise
    ///
    /// @param[in] octaves   number of fraction of noise to sum
    /// @param[in] x         float coordinate
    ///
    /// @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
    public subscript(octaves octaves: Int, x x: Double) -> Double {
        var output    = 0.0;
        var denom     = 0.0;
        var frequency = self.frequency;
        var amplitude = self.amplitude;

        for _ in 0..<octaves {
            output += amplitude * noise(x: x * frequency)
            denom  += amplitude

            frequency *= self.lacunarity
            amplitude *= self.persistence
        }

        return output / denom
    }

    /// Fractal/Fractional Brownian Motion (fBm) summation of 2D Perlin Simplex noise
    ///
    /// @param[in] octaves   number of fraction of noise to sum
    /// @param[in] x         x float coordinate
    /// @param[in] y         y float coordinate
    ///
    /// @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
    public subscript(octaves octaves: Int, x x: Double, y y: Double) -> Double {
        var output    = 0.0;
        var denom     = 0.0;
        var frequency = self.frequency;
        var amplitude = self.amplitude;

        for _ in 0..<octaves {
            output += amplitude * noise(x: x * frequency, y: y * frequency)
            denom  += amplitude

            frequency *= self.lacunarity
            amplitude *= self.persistence
        }

        return output / denom
    }

    /// 1D Perlin simplex noise
    ///
    ///  Takes around 74ns on an AMD APU.
    ///
    /// @param[in] x float coordinate
    ///
    /// @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
    public func noise(x: Double) -> Double {
        // No need to skew the input space in 1D

        // Corners coordinates (nearest integer values):
        let i0 = fastfloor(x);
        let i1 = i0 + 1;
        // Distances to corners (between 0 and 1):
        let x0 = x - Double(i0);
        let x1 = x0 - 1.0;

        // Calculate the contribution from the first corner
        let t0 = (1.0 - x0*x0) * (1.0 - x0*x0);
        //  if(t0 < 0.0f) t0 = 0.0f; // not possible
        let n0 = t0 * t0 * grad(hash(i0), x0);

        // Calculate the contribution from the second corner
        let t1 = (1.0 - x1*x1) * (1.0 - x1*x1);
        //  if(t1 < 0.0f) t1 = 0.0f; // not possible
        let n1 = t1 * t1 * grad(hash(i1), x1);

        // The maximum value of this noise is 8*(3/4)^4 = 2.53125
        // A factor of 0.395 scales to fit exactly within [-1,1]
        return 0.395 * (n0 + n1);
    }

    /// 2D Perlin simplex noise
    ///
    ///  Takes around 150ns on an AMD APU.
    ///
    /// @param[in] x float coordinate
    /// @param[in] y float coordinate
    ///
    /// @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
    public func noise(x: Double, y: Double) -> Double {
        // Skewing/Unskewing factors for 2D
        let F2 = 0.366025403;  // F2 = (sqrt(3) - 1) / 2
        let G2 = 0.211324865;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

        // Skew the input space to determine which simplex cell we're in
        let s = (x + y) * F2;  // Hairy factor for 2D
        let xs = x + s;
        let ys = y + s;
        let i = fastfloor(xs);
        let j = fastfloor(ys);

        // Unskew the cell origin back to (x,y) space
        let t = Double(i + j) * G2;
        let X0 = Double(i) - t;
        let Y0 = Double(j) - t;
        let x0 = x - X0;  // The x,y distances from the cell origin
        let y0 = y - Y0;

        // For the 2D case, the simplex shape is an equilateral triangle.
        // Determine which simplex we are in.
        let i1: UInt64, j1: UInt64;  // Offsets for second (middle) corner of simplex in (i,j) coords
        if x0 > y0 {   // lower triangle, XY order: (0,0)->(1,0)->(1,1)
            i1 = 1;
            j1 = 0;
        } else {   // upper triangle, YX order: (0,0)->(0,1)->(1,1)
            i1 = 0;
            j1 = 1;
        }

        // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
        // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
        // c = (3-sqrt(3))/6

        let x1 = x0 - Double(i1) + G2;            // Offsets for middle corner in (x,y) unskewed coords
        let y1 = y0 - Double(j1) + G2;
        let x2 = x0 - 1.0 + 2.0 * G2;   // Offsets for last corner in (x,y) unskewed coords
        let y2 = y0 - 1.0 + 2.0 * G2;

        // Calculate the contribution from the first corner
        var t0 = 0.5 - x0*x0 - y0*y0;
        let n0: Double
        if (t0 < 0.0) {
            n0 = 0.0;
        } else {
            t0 *= t0;
            n0 = t0 * t0 * grad(hash(i + hash(j)), x0, y0);
        }

        // Calculate the contribution from the second corner
        var t1 = 0.5 - x1*x1 - y1*y1;
        let n1: Double
        if (t1 < 0.0) {
            n1 = 0.0;
        } else {
            t1 *= t1;
            n1 = t1 * t1 * grad(hash(i + i1 + hash((j + j1))), x1, y1);
        }

        // Calculate the contribution from the third corner
        var t2 = 0.5 - x2*x2 - y2*y2;
        let n2: Double
        if t2 < 0.0 {
            n2 = 0.0;
        } else {
            t2 *= t2;
            n2 = t2 * t2 * grad(hash(i + 1 + hash(j + 1)), x2, y2);
        }

        // Add contributions from each corner to get the final noise value.
        // The result is scaled to return values in the interval [-1,1].
        return 45.23065 * (n0 + n1 + n2);
    }

}

/// Computes the largest integer value not greater than the float one
///
/// This method is faster than using (int32_t)std::floor(fp).
///
/// I measured it to be approximately twice as fast:
///  float:  ~18.4ns instead of ~39.6ns on an AMD APU),
///  double: ~20.6ns instead of ~36.6ns on an AMD APU),
/// Reference: http://www.codeproject.com/Tips/700780/Fast-floor-ceiling-functions
///
/// @param[in] fp    float input value
///
/// @return largest integer value not greater than fp
fileprivate func fastfloor(_ fp: Double) -> UInt64 {
    return UInt64(floor(fp))
}

/// Permutation table. This is just a random jumble of all numbers 0-255.
///
/// This produce a repeatable pattern of 256, but Ken Perlin stated
/// that it is not a problem for graphic texture as the noise features disappear
/// at a distance far enough to be able to see a repeatable pattern of 256.
///
/// This needs to be exactly the same for all instances on all platforms,
/// so it's easiest to just keep it as static explicit data.
/// This also removes the need for any initialisation of this class.
///
/// Note that making this an uint32_t[] instead of a uint8_t[] might make the
/// code run faster on platforms with a high penalty for unaligned single
/// byte addressing. Intel x86 is generally single-byte-friendly, but
/// some other CPUs are faster with 4-aligned reads.
/// However, a char[] is smaller, which avoids cache trashing, and that
/// is probably the most important aspect on most architectures.
/// This array is accessed a *lot* by the noise functions.
/// A vector-valued noise over 3D accesses it 96 times, and a
/// float-valued 4D noise 64 times. We want this to fit in the cache!
fileprivate let perm: [UInt8] = [
    151, 160, 137, 91, 90, 15,
    131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
    77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
    102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
    135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
    5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
    223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
    129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
    49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
];

/// Helper function to hash an integer using the above permutation table
///
///  This inline function costs around 1ns, and is called N+1 times for a noise of N dimension.
///
///  Using a real hash function would be better to improve the "repeatability of 256" of the above permutation table,
/// but fast integer Hash functions uses more time and have bad random properties.
///
/// @param[in] i Integer value to hash
///
/// @return 8-bits hashed value
fileprivate func hash(_ i: UInt64) -> UInt8 {
    return perm[Int(i & 0xFF)];
}

fileprivate func hash(_ i: UInt64) -> UInt64 {
    return UInt64(perm[Int(i & 0xFF)]);
}

/* NOTE Gradient table to test if lookup-table are more efficient than calculs
static const float gradients1D[16] = {
        -8.f, -7.f, -6.f, -5.f, -4.f, -3.f, -2.f, -1.f,
         1.f,  2.f,  3.f,  4.f,  5.f,  6.f,  7.f,  8.f
};
*/

/// Helper function to compute gradients-dot-residual vectors (1D)
///
/// @note that these generate gradients of more than unit length. To make
/// a close match with the value range of classic Perlin noise, the final
/// noise values need to be rescaled to fit nicely within [-1,1].
/// (The simplex noise functions as such also have different scaling.)
/// Note also that these noise functions are the most practical and useful
/// signed version of Perlin noise.
///
/// @param[in] hash  hash value
/// @param[in] x     distance to the corner
///
/// @return gradient value
fileprivate func grad(_ hash: UInt64, _ x: Double) -> Double {
    // Convert low 4 bits of hash code
    let h = hash & 0x0F

    // Gradient value 1.0, 2.0, ..., 8.0
    var grad = 1.0 + Double(h & 7)

    // Set a random sign for the gradient
    if (h & 8) != 0 { grad = -grad }

    // Multiply the gradient with the distance
    return (grad * x);
}

//fileprivate func grad(_ hash: UInt8, _ x: Double) -> Double {
//    return grad(UInt64(hash), x, y)
//}

/// Helper functions to compute gradients-dot-residual vectors (2D)
///
/// @param[in] hash  hash value
/// @param[in] x     x coord of the distance to the corner
/// @param[in] y     y coord of the distance to the corner
///
/// @return gradient value
fileprivate func grad(_ hash: UInt64, _ x: Double, _ y: Double) -> Double {
    // Convert low 3 bits of hash code
    let h = hash & 0x3F;

    // into 8 simple gradient directions,
    let u = h < 4 ? x : y;

    // and compute the dot product with (x,y).
    let v = h < 4 ? y : x;
    return Double((h & 1 != 0) ? -u : u) + ((h & 2 != 0) ? -2.0 * Double(v) : 2.0 * Double(v));
}

//fileprivate func grad(_ hash: UInt8, _ x: Double, _ y: Double) -> Double {
//    return grad(UInt64(hash), x, y)
//}
