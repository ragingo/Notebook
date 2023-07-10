#include <cstdio>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "types.h"

constexpr int BMP_HEADER_SIZE = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

int main() {
    const auto fileName = "catman.bmp";
    std::fstream stream { fileName, std::ios_base::in | std::ios_base::binary };

    stream.seekg(0, std::ios_base::beg);

    BITMAPFILEHEADER fileHeader = {};
    stream.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));

    BITMAPINFOHEADER infoHeader = {};
    stream.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

    const int dataSize = std::filesystem::file_size(fileName) - BMP_HEADER_SIZE;
    std::vector<std::uint8_t> pixels(dataSize);
    stream.read(reinterpret_cast<char*>(pixels.data()), dataSize);
    stream.close();

    const int bytesPerPixel = infoHeader.biBitCount / 8;
    const int stride = infoHeader.biWidth * bytesPerPixel;

    std::u8string result;

    for (int row = 0; row < infoHeader.biHeight; row++) {
        int reverse_row = (infoHeader.biHeight - 1) - row;
        for (int col = 0; col < infoHeader.biWidth; col++) {
            int offset = stride * reverse_row + col * bytesPerPixel;
            uint8_t r = pixels[offset + 0];
            uint8_t g = pixels[offset + 1];
            uint8_t b = pixels[offset + 2];

            r >>= 5;
            g >>= 5;
            b >>= 5;

            if (r > std::max(g, b)) {
                result += u8"🟥";
            }
            else if (g > std::max(r, b)) {
                result += u8"🟩";
            }
            else if (b > std::max(r, g)) {
                result += u8"🟦";
            }
            else {
                result += u8"⬜";
            }
        }
        result += u8"\n";
    }

    std::ofstream outputFile { "output.txt", std::ios_base::binary };
    outputFile << reinterpret_cast<const char*>(result.c_str());
    outputFile.close();

    return 0;
}
