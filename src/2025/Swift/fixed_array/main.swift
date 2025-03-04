import Foundation

func border() {
  print(String(repeating: "=", count: 100))
}

// https://forums.swift.org/t/t-uple-max-size/30635/4
// https://github.com/swiftlang/swift/issues/54154
// https://github.com/swiftlang/swift/pull/25818
// Tuple の最大要素数は 2^32-1 らしい。
// 以下の例では Int (64ビットCPUであれば64ビット符号付き整数) を100個並べている。
typealias Vec100 = (
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int,
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int,
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int,
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int,
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int,
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int,
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int,
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int,
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int,
  Int, Int, Int, Int, Int, Int, Int, Int, Int, Int
)

func test1() {
  let vec100 = (
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  )

  print(vec100)
  print(MemoryLayout.size(ofValue: vec100))
}

typealias Vec2 = (UInt8, UInt8)
typealias Vec4 = (Vec2, Vec2)
typealias Vec8 = (Vec4, Vec4)
typealias Vec16 = (Vec8, Vec8)
typealias Vec32 = (Vec16, Vec16)
typealias Vec64 = (Vec32, Vec32)
typealias Vec128 = (Vec64, Vec64)
typealias Vec256 = (Vec128, Vec128)
typealias Vec512 = (Vec256, Vec256)
typealias Vec1024 = (Vec512, Vec512)
// コンパイルに時間がかかったからこの辺でやめておく(^^;)
// typealias Vec2048 = (Vec1024, Vec1024)
// typealias Vec4096 = (Vec2048, Vec2048)
// typealias Vec8192 = (Vec4096, Vec4096)
// typealias Vec16384 = (Vec8192, Vec8192)
// typealias Vec32768 = (Vec16384, Vec16384)
// typealias Vec65536 = (Vec32768, Vec32768)
// typealias Vec131072 = (Vec65536, Vec65536)
// typealias Vec262144 = (Vec131072, Vec131072)
// typealias Vec524288 = (Vec262144, Vec262144)
// typealias Vec1048576 = (Vec524288, Vec524288)

enum Vec {
  static let empty2: (UInt8, UInt8) = (0, 0)
  static let empty4: (Vec2, Vec2) = (empty2, empty2)
  static let empty8: (Vec4, Vec4) = (empty4, empty4)
  static let empty16: (Vec8, Vec8) = (empty8, empty8)
  static let empty32: (Vec16, Vec16) = (empty16, empty16)
  static let empty64: (Vec32, Vec32) = (empty32, empty32)
  static let empty128: (Vec64, Vec64) = (empty64, empty64)
  static let empty256: (Vec128, Vec128) = (empty128, empty128)
  static let empty512: (Vec256, Vec256) = (empty256, empty256)
  static let empty1024: (Vec512, Vec512) = (empty512, empty512)
  // static let empty2048: (Vec1024, Vec1024) = (empty1024, empty1024)
  // static let empty4096: (Vec2048, Vec2048) = (empty2048, empty2048)
  // static let empty8192: (Vec4096, Vec4096) = (empty4096, empty4096)
  // static let empty16384: (Vec8192, Vec8192) = (empty8192, empty8192)
  // static let empty32768: (Vec16384, Vec16384) = (empty16384, empty16384)
  // static let empty65536: (Vec32768, Vec32768) = (empty32768, empty32768)
  // static let empty131072: (Vec65536, Vec65536) = (empty65536, empty65536)
  // static let empty262144: (Vec131072, Vec131072) = (empty131072, empty131072)
  // static let empty524288: (Vec262144, Vec262144) = (empty262144, empty262144)
  // static let empty1048576: (Vec524288, Vec524288) = (empty524288, empty524288)
}

func test2() {
  let size = MemoryLayout<Vec1024>.size
  border()
  print(size) // 1024 bytes

  let fillValue: UInt8 = 123
  var vec: Vec1024 = Vec.empty1024

  withUnsafeMutablePointer(to: &vec) { ptr in
    let p = ptr.withMemoryRebound(to: UInt8.self, capacity: size) { $0 }
    // ポインタを使って領域全体を一括で更新
    p.initialize(repeating: fillValue, count: size)
  }
  border()
  print(vec)

  let offset = 5
  withUnsafeMutablePointer(to: &vec) { ptr in
    let p = ptr.withMemoryRebound(to: UInt8.self, capacity: size) { $0 }
    // 特定の要素だけを更新
    p.advanced(by: offset).pointee = 99
  }

  border()
  print(vec)
}

enum Tuple {
  static func initialize<TupleType, ElementType>(
    _ data: inout TupleType,
    size: Int = MemoryLayout<TupleType>.size,
    value: ElementType
  ) {
    withUnsafeMutablePointer(to: &data) { ptr in
      let p = ptr.withMemoryRebound(to: ElementType.self, capacity: size) { $0 }
      p.initialize(repeating: value, count: size / MemoryLayout<ElementType>.size)
    }
  }

  static func update<TupleType, ElementType>(
    _ data: inout TupleType,
    size: Int = MemoryLayout<TupleType>.size,
    offset: Int,
    value: ElementType
  ) {
    withUnsafeMutablePointer(to: &data) { ptr in
      let p = ptr.withMemoryRebound(to: ElementType.self, capacity: size) { $0 }
      p.advanced(by: offset).pointee = value
    }
  }
}

func test3() {
  var vec128: Vec128 = Vec.empty128
  Tuple.initialize(&vec128, value: UInt8(5))
  Tuple.update(&vec128, offset: 2, value: UInt8(255))
  border()
  print(vec128)

  var vec32: Vec32 = Vec.empty32 // 32 bytes
  // Int -> 64 bit CPU なら 64 bit -> 8 bytes
  Tuple.initialize(&vec32, value: 1)
  // total 32 bytes / element 8 bytes = 4 elements
  Tuple.update(&vec32, offset: 2, value: 100)
  border()
  print(vec32)
}

func main() {
  // test1()
  // test2()
  test3()
}

main()
