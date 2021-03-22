
private let JPEG_TRUE: UInt8 = 1
private let JPEG_FALSE: UInt8 = 0

func decodeJpegFromFile(_ path: String) {
    var err_mgr = jpeg_error_mgr()
    var cinfo = jpeg_decompress_struct()
    rg_jpeg_create_decompress(&cinfo)
    cinfo.err = jpeg_std_error(&err_mgr)
    cinfo.do_block_smoothing = JPEG_FALSE
    cinfo.do_fancy_upsampling = JPEG_FALSE

    // jpeg_stdio_src(&cinfo, fp.get())
    jpeg_read_header(&cinfo, JPEG_TRUE)
    jpeg_start_decompress(&cinfo)

    var alloc_size = Int(cinfo.output_width * cinfo.output_height * UInt32(cinfo.output_components))
    var img_buff = UnsafeMutablePointer<UInt8>.allocate(capacity: alloc_size)
    var img_raw_ptr = UnsafeMutableRawPointer(img_buff)

    var lines: UInt32 = 0
    var stride = cinfo.output_width * UInt32(cinfo.output_components)
    var img_ptr = img_raw_ptr

    while cinfo.output_scanline < cinfo.output_height {
        lines = jpeg_read_scanlines(&cinfo, unsafeBitCast(img_raw_ptr, to: JSAMPARRAY.self), 1)
        img_ptr += Int(lines * stride)
    }

    jpeg_finish_decompress(&cinfo)
    jpeg_destroy_decompress(&cinfo)
}
