// https://gist.github.com/ragingo/d58e9ab682d72f4e668f4f019f4e91b9

import Foundation

private func runtimeFunction() {}

@const func static_assert(@const _ condition: Bool) -> Bool {
    if !condition {
        runtimeFunction()
        return false
    }
    return true
}

enum Math {
    @const static let pi = 3.141592653589793238462643383279502884197169399375105820974944592307816
    @const static let epsilon = Double.ulpOfOne

    @const static func add<T: Numeric>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs + rhs
    }

    @const static func sub<T: Numeric>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs - rhs
    }

    @const static func mul<T: Numeric>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs * rhs
    }

    @const static func div<T>(@const _ lhs: T, @const _ rhs: T) -> T where T: BinaryFloatingPoint {
        return lhs / rhs
    }

    @const static func div<T>(@const _ lhs: T, @const _ rhs: T) -> T where T: BinaryInteger {
        return lhs / rhs
    }

    @const static func abs<T>(@const _ value: T) -> T where T: Numeric, T: Comparable {
        return value < .zero ? -1 * value : value
    }

    @const static func min<T: Comparable>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs < rhs ? lhs : rhs
    }

    @const static func max<T: Comparable>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs < rhs ? rhs : lhs
    }

    @const static func clamp<T: Comparable>(@const value: T, @const from: T, @const to: T) -> T {
        @const let clamped1 = value < from ? from : value
        @const let clamped2 = clamped1 > to ? to : clamped1
        return clamped2
    }

    @const static func toRad(@const deg: Double) -> Double {
        return deg * pi / 180.0
    }

} // enum Math

@const let a: Int = Math.add(2, 4)
@const let b: Int = Math.sub(a, 1)
@const let c: Int = Math.mul(b, 2)
@const let d: Float = Math.div(Float(c), 2.0)
@const let e: Int = Int(d)
@const let f: Bool = static_assert(e == 5)

@const let g = Math.abs(-5)
@const let g_test = static_assert(g == 5)
@const let h = Math.min(10, 1)
@const let h_test = static_assert(h == 1)
@const let i = Math.max(10, 1)
@const let i_test = static_assert(i == 10)
@const let j = Math.clamp(value: 10, from: 1, to: 5)
@const let j_test = static_assert(j == 5)
@const let k = Math.pi
@const let l = Int(k * 100)
@const let l_test = static_assert(l == 314)

@const let m = Math.toRad(deg: 30.0)
@const let n = Math.pi / 6.0
@const let o = m - n
@const let p = Math.abs(o)
@const let p_test = static_assert(p <= Math.epsilon)
