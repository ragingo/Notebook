import Foundation

private let JPEG_TRUE: UInt8 = 1
private let JPEG_FALSE: UInt8 = 0

typealias ImagePointer = UnsafeMutablePointer<UInt8>

struct ImageInfo {
    let width: Int32
    let height: Int32
    let bitCount: Int32
    let image: ImagePointer
}

func decodeJpegFromMemory(_ data: Data, completed: @escaping (ImageInfo) -> ()) {
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

    if cinfo.num_components != 3 {
        print("[RgJpeg] [error] cinfo.num_components: \(cinfo.num_components)")
        return
    }

    jpeg_start_decompress(&cinfo)
    defer {
        jpeg_finish_decompress(&cinfo)
    }

    let src_stride = Int32(cinfo.output_width) * cinfo.num_components
    let dst_stride = Int32(cinfo.output_width) * 4

    let dst_img = ImagePointer.allocate(capacity: Int(dst_stride * Int32(cinfo.output_height)))
    let dst_img_ptr = UnsafeMutableRawPointer(dst_img)
    let dst_pixels = dst_img_ptr.assumingMemoryBound(to: UInt8.self)

    var dst_offset = 0
    var dst_row_ptr: JSAMPROW?

    // もっとコンパクトになるのかな・・・見直そう
    let src_img_row = ImagePointer.allocate(capacity: Int(src_stride))
    let src_img_row_ptr = UnsafeMutableRawPointer(src_img_row)
    let src_pixels = src_img_row_ptr.assumingMemoryBound(to: UInt8.self)
    var src_row_ptr: JSAMPROW? = src_pixels
    defer {
        src_img_row.deallocate()
    }

    while cinfo.output_scanline < cinfo.output_height {
        dst_offset = Int(cinfo.output_scanline) * Int(dst_stride)
        dst_row_ptr = dst_pixels + dst_offset

        jpeg_read_scanlines(&cinfo, &src_row_ptr, 1)

        var src_pix_offset = 0
        var dst_pix_offset = 0
        while src_pix_offset < src_stride {
            var s = src_row_ptr! + src_pix_offset
            var d = dst_row_ptr! + dst_pix_offset
            d[0] = s[0]
            d[1] = s[1]
            d[2] = s[2]
            d[3] = 0
            src_pix_offset += 3
            dst_pix_offset += 4
        }
    }

    let info = ImageInfo(
        width: Int32(cinfo.output_width),
        height: Int32(cinfo.output_height),
        bitCount: 32,
        image: dst_img
    )
    completed(info)
}
