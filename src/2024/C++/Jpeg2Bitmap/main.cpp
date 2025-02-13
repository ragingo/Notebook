#include <stdlib.h>
#include <iostream>
#include <string_view>
#include "bitmap.h"
#include "jpeg/jpeg.h"

void writeBitmap(const std::string_view filename, int width, int height, const std::vector<uint8_t>& pixels)
{
    BitmapFileHeader fileHeader{};
    BitmapInfoHeader infoHeader{};

    fileHeader.bfType = 0x4D42;
    fileHeader.bfSize = static_cast<uint32_t>(sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + pixels.size());
    fileHeader.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    infoHeader.biSize = sizeof(BitmapInfoHeader);
    infoHeader.biWidth = width;
    infoHeader.biHeight = -height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 32;
    infoHeader.biSizeImage = static_cast<uint32_t>(pixels.size());

    std::ofstream file(filename.data(), std::ios::binary);
    if (!file) {
        return;
    }

    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        return EXIT_FAILURE;
    }
    std::string_view jpegFileName = argv[1];
    std::string_view bmpFileName = argv[2];

    try {
        auto decoder = jpeg::JpegDecoder(jpegFileName.data());
        jpeg::DecodeResult result{};
        decoder.decode(result);
        writeBitmap(bmpFileName, result.width, result.height, result.pixels);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
