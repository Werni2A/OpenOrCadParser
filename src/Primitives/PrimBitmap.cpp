#include <cstdint>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Parser.hpp"
#include "Primitives/PrimBitmap.hpp"


namespace fs = std::filesystem;


PrimBitmap Parser::readPrimBitmap()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const size_t startOffset = mDs.getCurrentOffset();

    PrimBitmap obj;

    const uint32_t byteLength = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    obj.locX = mDs.readInt32();
    obj.locY = mDs.readInt32();

    spdlog::debug("locX = {}", obj.locX);
    spdlog::debug("locY = {}", obj.locY);

    obj.x2 = mDs.readInt32();
    obj.y2 = mDs.readInt32();
    obj.x1 = mDs.readInt32();
    obj.y1 = mDs.readInt32();

    spdlog::debug("x2 = {}", obj.x2);
    spdlog::debug("y2 = {}", obj.y2);
    spdlog::debug("x1 = {}", obj.x1);
    spdlog::debug("y1 = {}", obj.y1);

    obj.bmpWidth  = mDs.readUint32();
    obj.bmpHeight = mDs.readUint32();

    spdlog::debug("bmpWidth  = {}", obj.bmpWidth);
    spdlog::debug("bmpHeight = {}", obj.bmpHeight);

    const uint32_t dataSize = mDs.readUint32();

    spdlog::debug("dataSize = {}", dataSize);

    // Offset from the beginning of the image header to the actual image data
    const uint32_t offset = 40U;

    const uint32_t colorChannels = 3U; // Red, Green and Blue are always used

    if(dataSize != offset + obj.bmpWidth * obj.bmpHeight * colorChannels)
    {
        const std::string msg = fmt::format("{}: Unexpected bitmap data size of {} Byte!", __func__, dataSize);

        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    obj.rawImgData.clear();
    obj.rawImgData.reserve(dataSize);

    for(size_t i = 0U; i < dataSize; ++i)
    {
        obj.rawImgData.push_back(mDs.readUint8());
    }

    const fs::path filename = mCurrOpenFile.parent_path() / fmt::format("{}_img_{}.bmp",
        mCurrOpenFile.stem().string(), mImgCtr);

    spdlog::info("{}: Writing bitmap file to {}", __func__, filename.string());

    obj.writeBmpToFile(filename);

    ++mImgCtr;

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != 44U + dataSize)
    {
        throw FileFormatChanged("Bitmap");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


void PrimBitmap::writeBmpToFile(const fs::path& aFilePath) const
{
    std::ofstream bmp{aFilePath, std::ios::out | std::ios::binary};

    if(!bmp)
    {
        const std::string msg = fmt::format("{}: Can not open file for writing: {}",
            __func__, aFilePath.string());

        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    // Reconstruct header information that was not stored in the file container
    // See https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header
    // for further details on the meaning of each configuration value.
    struct BmpHeader
    {
        uint16_t magicBytes = 0x4d42;
        uint32_t bmpSize    =  0; // Will be set later
        uint32_t reserved   =  0;
        uint32_t offset     = 54; // @note Maybe this offset needs to be adjusted according to the bitmap
    } header;

    // @note We can not derive the header size automatically as the compiler
    //       inserts additional space into the structure for better alignment
    const uint32_t headerSize = 14U; // Size in Byte

    header.bmpSize = rawImgData.size() + headerSize;

    // Start writing the file
    bmp.write(reinterpret_cast<const char*>(&header.magicBytes), sizeof(header.magicBytes));
    bmp.write(reinterpret_cast<const char*>(&header.bmpSize), sizeof(header.bmpSize));
    bmp.write(reinterpret_cast<const char*>(&header.reserved), sizeof(header.reserved));
    bmp.write(reinterpret_cast<const char*>(&header.offset), sizeof(header.offset));

    for(const auto& data : rawImgData)
    {
        bmp.write(reinterpret_cast<const char*>(&data), sizeof(data));
    }

    bmp.close();
}