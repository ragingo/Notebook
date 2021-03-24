import Foundation

private let JPEG_TRUE: UInt8 = 1
private let JPEG_FALSE: UInt8 = 0


typealias ImagePointer = UnsafeMutablePointer<UInt8>

func decodeJpegFromMemory(_ data: Data, completed: @escaping (ImagePointer) -> ()) {
    var err_mgr = jpeg_error_mgr()
    var cinfo = jpeg_decompress_struct()
    cinfo.err = jpeg_std_error(&err_mgr)
    cinfo.do_block_smoothing = JPEG_FALSE
    cinfo.do_fancy_upsampling = JPEG_FALSE
    rg_jpeg_create_decompress(&cinfo)
    defer {
        jpeg_destroy_decompress(&cinfo)
    }

    data.withUnsafeBytes {
        if let addr = $0.baseAddress {
            jpeg_mem_src(&cinfo, addr.assumingMemoryBound(to: UInt8.self), UInt32($0.count))
        }
    }
    jpeg_read_header(&cinfo, JPEG_TRUE)
    jpeg_start_decompress(&cinfo)
    defer {
        jpeg_finish_decompress(&cinfo)
    }

    let stride = Int32(cinfo.output_width) * cinfo.output_components
    let alloc_size = Int(stride * Int32(cinfo.output_height))
    let img = ImagePointer.allocate(capacity: alloc_size)
    let img_ptr = UnsafeMutableRawPointer(img)
    let pixels = img_ptr.assumingMemoryBound(to: UInt8.self)

    while cinfo.output_scanline < cinfo.output_height {
        print("output_scanline: \(cinfo.output_scanline)")
        var row_ptr: JSAMPROW? = pixels + Int(cinfo.output_scanline) * Int(stride)
        jpeg_read_scanlines(&cinfo, &row_ptr, 1)
    }

    completed(img)

    img.deallocate()
}
