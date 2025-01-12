#pragma once
#include <memory>
#include "BitStreamReader.h"
#include "JpegParser.h"

namespace jpeg {

class JpegDecoder final
{
public:
    JpegDecoder(const char* fileName);
    ~JpegDecoder();

    void decode();

private:
    JpegParser m_Parser;
    std::unique_ptr<BitStreamReader> m_BitStreamReader;
};

} // namespace jpeg
