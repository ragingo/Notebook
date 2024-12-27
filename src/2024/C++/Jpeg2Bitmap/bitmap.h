#pragma once
#include <fstream>
#include <iostream>
#include <string>

struct BITMAPFILEHEADER
{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
};

struct BITMAPINFOHEADER
{
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};

void createBitmapFile(
    const std::string& fileName,
    const BITMAPFILEHEADER& fileHeader,
    const BITMAPINFOHEADER& infoHeader,
    const uint8_t* data
)
{
    std::ofstream bmpFile(fileName, std::ios::binary);
    if (!bmpFile) {
        std::cerr << "Error: Cannot create " << fileName << std::endl;
        return;
    }
    bmpFile.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    bmpFile.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    bmpFile.write(reinterpret_cast<const char*>(data), infoHeader.biSizeImage);
}
