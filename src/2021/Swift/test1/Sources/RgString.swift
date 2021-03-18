final class RgString {
    private var buffer: UnsafeMutablePointer<UInt16>

    var ptr: UnsafePointer<UInt16> {
        UnsafePointer(self.buffer)
    }

    init(_ s: String) {
        let nullTerminatedString = s + "\0"
        var arr = Array<UInt16>(nullTerminatedString.utf16)
        self.buffer = UnsafeMutablePointer<UInt16>.allocate(capacity: arr.count)
        self.buffer.initialize(from: &arr, count: arr.count)
    }

    deinit {
        self.buffer.deallocate()
    }
}
