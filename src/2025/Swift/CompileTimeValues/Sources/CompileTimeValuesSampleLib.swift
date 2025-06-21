// https://gist.github.com/ragingo/d58e9ab682d72f4e668f4f019f4e91b9

// このコードは、 swift build ではコンパイルできない！
// swiftc を直接使ってビルドすること。

import Foundation

func runtimeFunction() {}

@const func static_assert(@const _ condition: Bool) -> Bool {
    if !condition {
        runtimeFunction()
        //#error("") // compile error
        return false
    }
    return true
}

//enum Math { // クラッシュするからトップレベル関数として使うことにする
@const func add<T: BinaryInteger>(@const _ lhs: T, @const _ rhs: T) -> T {
  return lhs + rhs
}

@const func sub<T: BinaryInteger>(@const _ lhs: T, @const _ rhs: T) -> T {
  return lhs - rhs
}

@const func mul<T: BinaryInteger>(@const _ lhs: T, @const _ rhs: T) -> T {
  return lhs * rhs
}

@const func div<T: FloatingPoint>(@const _ lhs: T, @const _ rhs: T) -> T {
  return lhs / rhs
}

@const func abs<T>(@const _ value: T) -> T where T: BinaryInteger {
    return value < T.zero ? -1 * value : value
}

@const func abs<T>(@const _ value: T) -> T where T: FloatingPoint {
    return value < T.zero ? -1 * value : value
}

@const func min<T: BinaryInteger>(@const _ lhs: T, @const _ rhs: T) -> T {
    return lhs < rhs ? lhs : rhs
}

@const func max<T: BinaryInteger>(@const _ lhs: T, @const _ rhs: T) -> T {
    return lhs < rhs ? rhs : lhs
}

@const func clamp<T: BinaryInteger>(@const value: T, @const from: T, @const to: T) -> T {
    //@const let a: T = max(value, from) // compile error
    @const let clamped1 = value < from ? from : value
    @const let clamped2 = clamped1 < to ? clamped1 : to
    return clamped2
}

@const let pi = 3.141592653589793238462643383279502884197169399375105820974944592307816
@const let epsilon = Double.ulpOfOne

@const func toRad(@const deg: Double) -> Double {
    return deg * pi / 180.0
}

//} // enum Math

@const let a: Int = add(2, 4)
@const let b: Int = sub(a, 1)
@const let c: Int = mul(b, 2)
@const let d: Float = div(Float(c), 2.0)
@const let e: Int = Int(d)
@const let f: Bool = static_assert(e == 5)
//@const let f_test: Bool = static_assert(e == 6) // compile error

@const let g = abs(-5)
@const let g_test = static_assert(g == 5)
@const let h = min(10, 1)
@const let h_test = static_assert(h == 1)
@const let i = max(10, 1)
@const let i_test = static_assert(i == 10)
@const let j = clamp(value: 10, from: 1, to: 5)
@const let j_test = static_assert(j == 5)
//@const let k = pi() * 100 // compile error
@const let k = pi
@const let l = Int(k * 100)
@const let l_test = static_assert(l == 314)

@const let m = toRad(deg: 30.0)
@const let n = pi / 6.0
@const let o = m - n
@const let p = abs(o)
@const let p_test = static_assert(p <= epsilon)

// 関数は void 以外を返さないとだめらしい。省略もできない。
//@const let z: Void = static_assert(y == 120) // compile error
//@const let _ = static_assert(y == 120) // compile error
//@const _ = static_assert(y == 120) // compile error
//static_assert(y == 120) // compile error