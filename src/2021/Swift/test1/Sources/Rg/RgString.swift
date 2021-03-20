final class RgString {
    private let originalString: String
    private var buffer: UnsafeMutablePointer<UInt16>

    var ptr: UnsafePointer<UInt16> {
        UnsafePointer(self.buffer)
    }

    private(set) var length: Int32

    init(_ s: String) {
        self.originalString = s
        let nullTerminatedString = s + "\0"
        var arr = Array<UInt16>(nullTerminatedString.utf16)
        self.length = Int32(arr.count)
        self.buffer = UnsafeMutablePointer<UInt16>.allocate(capacity: arr.count)
        self.buffer.initialize(from: &arr, count: arr.count)
    }

    deinit {
        self.buffer.deallocate()
    }
}
