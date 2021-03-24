import Foundation

private let JPEG_TRUE: UInt8 = 1
private let JPEG_FALSE: UInt8 = 0

func decodeJpegFromMemory(_ data: Data, completed: @escaping (UnsafeMutableRawPointer) -> ()) {
    var err_mgr = jpeg_error_mgr()
    var cinfo = jpeg_decompress_struct()
    rg_jpeg_create_decompress(&cinfo)
    cinfo.err = jpeg_std_error(&err_mgr)
    cinfo.do_block_smoothing = JPEG_FALSE
    cinfo.do_fancy_upsampling = JPEG_FALSE

    data.withUnsafeBytes {
        if let addr = $0.baseAddress {
            jpeg_mem_src(&cinfo, addr.assumingMemoryBound(to: UInt8.self), UInt32($0.count))
        }
    }
    jpeg_read_header(&cinfo, JPEG_TRUE)
    jpeg_start_decompress(&cinfo)

    let alloc_size = Int(cinfo.output_width * cinfo.output_height * UInt32(cinfo.output_components))
    let img_buff = UnsafeMutablePointer<UInt8>.allocate(capacity: alloc_size)
    let img_raw_ptr = UnsafeMutableRawPointer(img_buff)

    var lines: UInt32 = 0
    let stride = cinfo.output_width * UInt32(cinfo.output_components)

     let pixels = img_raw_ptr.assumingMemoryBound(to: UInt8.self)

    while cinfo.output_scanline < cinfo.output_height {
        print("lines: \(lines), output_scanline: \(cinfo.output_scanline)")
        var row_ptr: JSAMPROW? = pixels + Int(cinfo.output_scanline) * Int(stride)
        jpeg_read_scanlines(&cinfo, &row_ptr, 1)
    }

    completed(img_raw_ptr)

    jpeg_finish_decompress(&cinfo)
    jpeg_destroy_decompress(&cinfo)
}
