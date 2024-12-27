#include <bit>
#include <fstream>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#include "jpeg.h"

int main()
{
    const std::string jpegFileName = "D:\\temp\\images\\catman.jpg";
    const std::string bmpFileName = "D:\\temp\\images\\catman.jpg.bmp";

    try {
        auto parser = jpg::JpegParser(jpegFileName);
        parser.parse();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
