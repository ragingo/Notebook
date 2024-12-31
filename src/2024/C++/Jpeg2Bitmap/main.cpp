#include <iostream>
#include <string>
#include "jpeg/jpeg.h"

int main()
{
    const char* jpegFileName = "D:\\temp\\images\\catman.jpg";

    try {
        auto decoder = jpeg::JpegDecoder(jpegFileName);
        decoder.decode();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
