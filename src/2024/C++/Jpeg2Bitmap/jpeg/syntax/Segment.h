#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <variant>
#include <vector>
#include "Marker.h"

namespace jpeg::segments {

    enum class QuantizationTableID : uint8_t
    {
        LUMINANCE = 0x0,
        CHROMINANCE = 0x1,
        RESERVED_2 = 0x2,
        RESERVED_3 = 0x3,
    };

    enum class HuffmanTableID : uint8_t
    {
        LUMINANCE = 0x0,
        CHROMINANCE = 0x1,
        RESERVED_2 = 0x2,
        RESERVED_3 = 0x3,
    };

    enum class ComponentID : uint8_t
    {
        Y = 1,
        Cb = 2,
        Cr = 3,
        I = 4,
        Q = 5,
    };

    enum class ColorSpace
    {
        GRAYSCALE,
        YCbCr,
        YIQ,
        CMYK,
        UNKNOWN,
    };

    enum class YUVFormat
    {
        YUV444,
        YUV422,
        YUV420,
        YUV411,
        YUV410,
        YUV400,
        UNKNOWN,
    };

    struct Segment
    {
        Marker marker;
        virtual ~Segment() = default;
    };

    // Start of Image
    // SOI マーカーは、JPEG ファイルの開始を示す。
    // MotionJPEG では、連続する JPEG 画像の区切りを示すために使用される。
    struct SOI : public Segment
    {
    };

    // JFIF Extension APP0 Marker Segment
    struct APP0 : public Segment
    {
        enum Version : uint16_t
        {
            UNKNOWN = 0,
            V1_1 = 0x101,
            V1_2 = 0x102,
        };

        enum class Units : uint8_t
        {
            NONE = 0,
            DPI = 1,
            DPCM = 2,
        };

        // length
        uint16_t length = 0;
        // identifier
        std::array<char, 5> identifier;
        // version
        Version version{};
        // units
        Units units{};
        // Xdensity
        uint16_t xDensity = 0;
        // Ydensity
        uint16_t yDensity = 0;
        // Xthumbnail
        uint8_t thumbnailWidth = 0;
        // Ythumbnail
        uint8_t thumbnailHeight = 0;

        // TODO: thumbnail (RGB 24bit, Xthumbnail * Ythumbnail * 3 pixels)
    };

    // B.2.4.1 Quantization table-specification syntax
    struct DQT : public Segment
    {
        enum class Precision : uint8_t
        {
            BITS_8 = 0,
            BITS_16 = 1,
        };

        // Lq: Quantization table definition length
        uint16_t length = 0;
        // Pq: Quantization table element precision
        Precision precision : 4{};
        // Tq: Quantization table destination identifier
        QuantizationTableID tableID : 4{};

        using Bits8Table = std::array<uint8_t, 64>;
        using Bits16Table = std::array<uint16_t, 64>;
        // Qk: Quantization table element
        std::variant<Bits8Table, Bits16Table> table{};
    };

    // B.2.2 Frame header syntax
    // Baseline DCT
    struct SOF0 : public Segment
    {
        // Lf: Frame header length
        uint16_t length = 0;
        // P: Sample precision
        uint8_t precision = 0;
        // Y: Number of lines
        uint16_t height = 0;
        // X: Number of samples per line
        uint16_t width = 0;
        // Nf: Number of image components in frame
        uint8_t numComponents = 0;

        struct Component
        {
            // Ci: Component identifier
            ComponentID id{};
            // Hi: Horizontal sampling factor
            uint8_t horizonalSamplingFactor : 4{};
            // Vi: Vertical sampling factor
            uint8_t verticalSamplingFactor : 4{};
            // Tqi: Quantization table destination selector
            QuantizationTableID tableID{};
        };
        // Component-specification parameters
        std::vector<Component> components{};
    };

    // B.2.4.2 Huffman table-specification syntax
    struct DHT : public Segment
    {
        enum class TableClass : uint8_t
        {
            DC_OR_LOSSLESS = 0,
            AC = 1,
        };

        // Lh: Huffman table definition length
        uint16_t length = 0;
        // Tc: Table class
        TableClass tableClass : 4{};
        // Th: Huffman table destination identifier
        HuffmanTableID tableID : 4{};
        // Li: Number of Huffman codes of length i
        // BITS の要素が Li
        std::array<uint8_t, 16> counts{};
        // Vi,j: Value associated with each Huffman code
        // HUFFVAL の要素が Vi,j
        std::vector<uint8_t> symbols{};
    };

    // B.2.3 Scan header syntax
    struct SOS : public Segment
    {
        // Ls: Scan header length
        uint16_t length;
        // Ns: Number of image components in scan
        uint8_t numComponents;

        struct Component
        {
            // Csj: Scan component selector
            ComponentID componentSelector{};
            // Tdj: DC entropy coding table destination selector
            HuffmanTableID dcSelector : 4{};
            // Taj: AC entropy coding table destination selector
            HuffmanTableID acSelector : 4{};
        };
        // Scan component-specification parameters
        std::vector<Component> components{};

        // Ss: Start of spectral or predictor selection
        uint8_t spectralSelectionStart = 0;
        // Se: End of spectral selection
        uint8_t spectralSelectionEnd = 0;
        // Ah: Successive approximation bit position high
        uint8_t successiveApproximation = 0;
    };

    // B.2.4.4 Restart interval
    struct DRI : public Segment
    {
        // Lr: Define restart interval segment length
        uint16_t length = 0;
        // Ri: Restart interval
        uint16_t restartInterval = 0;
    };

    // B.2.5 Define number of lines
    struct DNL : public Segment
    {
        // Ld: Define number of lines segment length
        uint16_t length = 0;
        // NL: Number of lines
        uint16_t lines = 0;
    };

    // End of Image
    // EOI マーカーは、JPEG ファイルの終了を示す。
    // MotionJPEG では、連続する JPEG 画像の区切りを示すために使用される。
    struct EOI : public Segment {};

    struct APP1 : public Segment
    {
        uint16_t length = 0;
    };

    struct APP13 : public Segment
    {
        uint16_t length = 0;
    };

    struct APP14 : public Segment
    {
        uint16_t length = 0;
    };

    // B.2.4.5 Comment syntax
    struct COM : public Segment
    {
        // Lc: Comment segment length
        uint16_t length = 0;
        // Cmi: Comment byte
        std::vector<std::byte> comment{};

        std::string_view getComment() const
        {
            return { reinterpret_cast<const char*>(comment.data()), comment.size() };
        }
    };

} // namespace jpg::segments
