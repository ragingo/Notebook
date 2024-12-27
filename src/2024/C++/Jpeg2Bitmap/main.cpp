#include <bit>
#include <fstream>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#include "jpeg.h"

std::vector<uint8_t> readBytes(std::ifstream& stream, int count)
{
    std::vector<uint8_t> bytes(count);
    stream.read(reinterpret_cast<char*>(bytes.data()), count);
    return bytes;
}

jpg::Markers readMarker(std::ifstream& stream)
{
    uint16_t marker;
    stream.read(reinterpret_cast<char*>(&marker), sizeof(marker));
    marker = std::byteswap(marker);
    return static_cast<jpg::Markers>(marker);
}

void parseSOI(std::ifstream& stream)
{
    auto soi = jpg::segments::SOI{};
    soi.reserved = jpg::marker_upper(jpg::Markers::SOI);
    soi.marker = jpg::marker_lower(jpg::Markers::SOI);
    std::cout << jpg::to_string(soi) << std::endl;
}

void parseAPP0(std::ifstream& stream)
{
    auto app0 = jpg::segments::APP0{};
    app0.reserved = jpg::marker_upper(jpg::Markers::APP0);
    app0.marker = jpg::marker_lower(jpg::Markers::APP0);
    stream.read(reinterpret_cast<char*>(&app0.length), sizeof(app0.length));
    app0.length = std::byteswap(app0.length);
    int remain = app0.length - sizeof(app0.length);
    if (remain >= sizeof(app0.identifier)) {
        stream.read(reinterpret_cast<char*>(&app0.identifier), sizeof(app0.identifier));
        remain -= sizeof(app0.identifier);
    }
    if (remain >= sizeof(app0.version)) {
        stream.read(reinterpret_cast<char*>(&app0.version), sizeof(app0.version));
        app0.version = std::byteswap(app0.version);
        remain -= sizeof(app0.version);
    }
    if (remain >= sizeof(app0.units)) {
        stream.read(reinterpret_cast<char*>(&app0.units), sizeof(app0.units));
        remain -= sizeof(app0.units);
    }
    if (remain >= sizeof(app0.xDensity) + sizeof(app0.yDensity)) {
        stream.read(reinterpret_cast<char*>(&app0.xDensity), sizeof(app0.xDensity));
        stream.read(reinterpret_cast<char*>(&app0.yDensity), sizeof(app0.yDensity));
        app0.xDensity = std::byteswap(app0.xDensity);
        app0.yDensity = std::byteswap(app0.yDensity);
        remain -= sizeof(app0.xDensity) + sizeof(app0.yDensity);
    }
    if (remain >= sizeof(app0.thumbnailWidth) + sizeof(app0.thumbnailHeight)) {
        stream.read(reinterpret_cast<char*>(&app0.thumbnailWidth), sizeof(app0.thumbnailWidth));
        stream.read(reinterpret_cast<char*>(&app0.thumbnailHeight), sizeof(app0.thumbnailHeight));
        remain -= sizeof(app0.thumbnailWidth) + sizeof(app0.thumbnailHeight);
    }

    std::cout << jpg::to_string(app0) << std::endl;
}

void parseDQT(std::ifstream& stream)
{
    auto dqt = jpg::segments::DQT{};
    dqt.reserved = jpg::marker_upper(jpg::Markers::DQT);
    dqt.marker = jpg::marker_lower(jpg::Markers::DQT);
    stream.read(reinterpret_cast<char*>(&dqt.length), sizeof(dqt.length));
    dqt.length = std::byteswap(dqt.length);
    int remain = dqt.length - sizeof(dqt.length);
    if (remain >= 1) {
        uint8_t value;
        stream.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
        dqt.precision = value >> 4;
        dqt.tableID = value & 0x0F;
        remain--;
    }
    if (remain >= sizeof(dqt.table)) {
        stream.read(reinterpret_cast<char*>(&dqt.table), sizeof(dqt.table));
        remain -= sizeof(dqt.table);
    }

    std::cout << jpg::to_string(dqt) << std::endl;
}

void parseJpeg(std::ifstream& stream)
{
    auto marker = readMarker(stream);

    if (marker != jpg::Markers::SOI) {
        std::cerr << "Error: Invalid JPEG file" << std::endl;
        return;
    }

    while (marker != jpg::Markers::EOI) {
        switch (marker) {
        case jpg::Markers::SOI:
            parseSOI(stream);
            break;
        case jpg::Markers::APP0:
            parseAPP0(stream);
            break;
        case jpg::Markers::DQT:
            parseDQT(stream);
            break;
        default:
            break;
        }
        marker = readMarker(stream);
    }
}

int main()
{
    const std::string jpegFileName = "D:\\temp\\images\\catman.jpg";
    const std::string bmpFileName = "D:\\temp\\images\\catman.jpg.bmp";

    std::ifstream stream(jpegFileName, std::ios::binary);
    if (!stream) {
        std::cerr << "Error: Cannot open " << jpegFileName << std::endl;
        return 1;
    }

    parseJpeg(stream);

    return 0;
}
