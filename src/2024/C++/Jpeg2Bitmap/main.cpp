#include <iostream>
#include <string>

#include "jpeg.h"

int main()
{
    const std::string jpegFileName = "D:\\temp\\images\\catman.jpg";
    const std::string bmpFileName = "D:\\temp\\images\\catman.jpg.bmp";

    try {
        auto decoder = jpg::JpegDecoder(jpegFileName);
        decoder.decode();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
