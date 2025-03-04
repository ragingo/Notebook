import Foundation

func border() {
    print(String(repeating: "-", count: 100))
}

extension Array {
    func printBufferInfo(prefix: String) {
        withUnsafeBufferPointer {
            print(prefix, "addr=\($0.baseAddress!) content: \(self)")
        }
    }
}

func updateFirst(_ arr: inout [Int]) {
    arr[0] = 10
}

func moveAndUpdate(_ arr: consuming [Int]) -> [Int] {
    arr[0] = 11
    return arr
}

func cowUpdateFirst(_ arr: [Int]) -> [Int] {
    var arr2 = arr
    arr2[0] = 12
    return arr2
}

let a = [1,2]
var b = a

border()
print("""
let a = [1,2]
var b = a
""")

a.printBufferInfo(prefix: "a:")
b.printBufferInfo(prefix: "b:")

border()
b += [3]
print("b += [3]")
a.printBufferInfo(prefix: "a:")
b.printBufferInfo(prefix: "b:")

border()
let c = Array(b.prefix(3))
print("var c = Array(b.prefix(3))")
b.printBufferInfo(prefix: "b:")
c.printBufferInfo(prefix: "c:")

border()
let d = Array(b.prefix(2))
print("let d = Array(b.prefix(2))")
b.printBufferInfo(prefix: "b:")
d.printBufferInfo(prefix: "d:")

border()
print("updateFirst(&b)")
updateFirst(&b)
b.printBufferInfo(prefix: "b:")
c.printBufferInfo(prefix: "c:")

border()
print("moveAndUpdate(b)")
let e = moveAndUpdate(b)
b.printBufferInfo(prefix: "b:")
e.printBufferInfo(prefix: "e:")

border()
print("cowUpdateFirst(b)")
let f = cowUpdateFirst(b)
b.printBufferInfo(prefix: "b:")
f.printBufferInfo(prefix: "f:")


border()
var arr = Array(repeating: 0, count: 100)
var i = 0
//for e in arr.enumerated() {
for e in arr {
    arr[i] = i
    i += 1
    arr.printBufferInfo(prefix: "")
}
