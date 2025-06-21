// https://gist.github.com/ragingo/d58e9ab682d72f4e668f4f019f4e91b9

import Foundation

private func runtimeFunction() {}

@const @discardableResult public func static_assert(@const _ condition: Bool) -> Bool {
    if !condition {
        runtimeFunction()
        return false
    }
    return true
}

public enum Math {
    @const public static let pi = 3.141592653589793238462643383279502884197169399375105820974944592307816
    @const public static let epsilon = Double.ulpOfOne

    @const public static func add<T: Numeric>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs + rhs
    }

    @const public static func sub<T: Numeric>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs - rhs
    }

    @const public static func mul<T: Numeric>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs * rhs
    }

    @const public static func div<T>(@const _ lhs: T, @const _ rhs: T) -> T where T: BinaryFloatingPoint {
        return lhs / rhs
    }

    @const public static func div<T>(@const _ lhs: T, @const _ rhs: T) -> T where T: BinaryInteger {
        return lhs / rhs
    }

    @const public static func abs<T>(@const _ value: T) -> T where T: Numeric, T: Comparable {
        return value < .zero ? -1 * value : value
    }

    @const public static func min<T: Comparable>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs < rhs ? lhs : rhs
    }

    @const public static func max<T: Comparable>(@const _ lhs: T, @const _ rhs: T) -> T {
        return lhs < rhs ? rhs : lhs
    }

    @const public static func clamp<T: Comparable>(@const value: T, @const from: T, @const to: T) -> T {
        @const let clamped1 = value < from ? from : value
        @const let clamped2 = clamped1 > to ? to : clamped1
        return clamped2
    }

    @const public static func toRad(@const deg: Double) -> Double {
        return deg * pi / 180.0
    }

    @const public static func toDeg(@const rad: Double) -> Double {
        return rad * 180.0 / pi
    }

} // enum Math


#if true

// @const func testAdd() {
//     @const let a = Math.add(2, 3)
//     @const let r = static_assert(a == 6) // コンパイルエラーにならない
// }

@const let add_result = Math.add(2, 3)
@const let add_assertion = static_assert(add_result == 5)

@const let sub_result = Math.sub(5, 3)
@const let sub_assertion = static_assert(sub_result == 2)

@const let mul_result = Math.mul(2, 3)
@const let mul_assertion = static_assert(mul_result == 6)

@const let div_result = Math.div(6, 3)
@const let div_assertion = static_assert(div_result == 2)

@const let abs_result = Math.abs(-5)
@const let abs_assertion = static_assert(abs_result == 5)

@const let min_result = Math.min(2, 3)
@const let min_assertion = static_assert(min_result == 2)

@const let max_result = Math.max(2, 3)
@const let max_assertion = static_assert(max_result == 3)

@const let clamp_result = Math.clamp(value: 5, from: 2, to: 4)
@const let clamp_assertion = static_assert(clamp_result == 4)

@const let toRad_result = Math.toRad(deg: 180)
@const let toRad_assertion = static_assert(toRad_result == Math.pi)

@const let toDeg_result = Math.toDeg(rad: Math.pi)
@const let toDeg_assertion = static_assert(toDeg_result == 180.0)

#endif
