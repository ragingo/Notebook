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

    // dst (bgra)
    let dst_stride = Int(cinfo.output_width) * 4
    let dst_img = ImagePointer.allocate(capacity: dst_stride * Int(cinfo.output_height))
    let dst_img_ptr = UnsafeMutableRawPointer(dst_img)
    let dst_pixels = dst_img_ptr.assumingMemoryBound(to: JSAMPLE.self)

    // src (rgb)
    let src_stride = Int(cinfo.output_width) * 3
    let src_img_row = ImagePointer.allocate(capacity: src_stride)
    let src_img_row_ptr = UnsafeMutableRawPointer(src_img_row)
    let src_pixels = src_img_row_ptr.assumingMemoryBound(to: JSAMPLE.self)
    var src_row_ptr: JSAMPROW? = src_pixels
    defer {
        src_img_row.deallocate()
    }

    while cinfo.output_scanline < cinfo.output_height {
        let dst_row_ptr = dst_pixels + (Int(cinfo.output_scanline) * dst_stride)

        if jpeg_read_scanlines(&cinfo, &src_row_ptr, 1) != 1 {
            break
        }

        guard let src_row_ptr = src_row_ptr else {
            continue
        }

        var s_offset = 0
        var d_offset = 0
        while s_offset < src_stride {
            dst_row_ptr[d_offset + 0] = src_row_ptr[s_offset + 2]
            dst_row_ptr[d_offset + 1] = src_row_ptr[s_offset + 1]
            dst_row_ptr[d_offset + 2] = src_row_ptr[s_offset + 0]
            dst_row_ptr[d_offset + 3] = 0
            s_offset += 3
            d_offset += 4
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
