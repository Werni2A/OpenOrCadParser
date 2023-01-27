#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Primitives/PrimBitmap.hpp"


namespace fs = std::filesystem;


void PrimBitmap::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const size_t startOffset = mDs.get().getCurrentOffset();

    const uint32_t byteLength = mDs.get().readUint32();

    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    locX = mDs.get().readInt32();
    locY = mDs.get().readInt32();

    spdlog::trace("locX = {}", locX);
    spdlog::trace("locY = {}", locY);

    x2 = mDs.get().readInt32();
    y2 = mDs.get().readInt32();
    x1 = mDs.get().readInt32();
    y1 = mDs.get().readInt32();

    spdlog::trace("x2 = {}", x2);
    spdlog::trace("y2 = {}", y2);
    spdlog::trace("x1 = {}", x1);
    spdlog::trace("y1 = {}", y1);

    bmpWidth  = mDs.get().readUint32();
    bmpHeight = mDs.get().readUint32();

    spdlog::trace("bmpWidth  = {}", bmpWidth);
    spdlog::trace("bmpHeight = {}", bmpHeight);

    const uint32_t dataSize = mDs.get().readUint32();

    spdlog::trace("dataSize = {}", dataSize);

    rawImgData.clear();
    rawImgData.reserve(dataSize);

    for(size_t i = 0U; i < dataSize; ++i)
    {
        rawImgData.push_back(mDs.get().readUint8());
    }

    // fs::path filename = mCurrOpenFile.parent_path() / fmt::format("{}_img_{}.bmp", // @todo fixme
    //    mCurrOpenFile.stem().string(), mImgCtr);
    fs::path filename = fs::temp_directory_path() / fmt::format("{}_img_{}.bmp", 0, 0);

    filename = writeImgToFile(filename);

    spdlog::info("{}: Wrote bitmap file to {}", __func__, filename.string());

    // @todo fixme
    // ++mImgCtr;

    if(mDs.get().getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.get().getCurrentOffset());
    }

    if(byteLength != 44U + dataSize)
    {
        throw FileFormatChanged("Bitmap");
    }

    readPreamble();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}


// Returns path to the written image file
static fs::path writeBmpFile(fs::path aFilePath, const std::vector<uint8_t>& aRawImgData)
{
    aFilePath.replace_extension(".bmp");

    std::ofstream img{aFilePath, std::ios::out | std::ios::binary};

    if(!img)
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

    header.bmpSize = aRawImgData.size() + headerSize;

    // Start writing the file
    img.write(reinterpret_cast<const char*>(&header.magicBytes), sizeof(header.magicBytes));
    img.write(reinterpret_cast<const char*>(&header.bmpSize), sizeof(header.bmpSize));
    img.write(reinterpret_cast<const char*>(&header.reserved), sizeof(header.reserved));
    img.write(reinterpret_cast<const char*>(&header.offset), sizeof(header.offset));

    spdlog::debug("Writing header is done");

    for(const auto& data : aRawImgData)
    {
        img.write(reinterpret_cast<const char*>(&data), sizeof(data));
    }

    img.close();

    return aFilePath;
}


// aFilePath is the requested path to the image file, but the function will change the file
// extension, depending on the corresponding file type that was found.
// Returns path to the actually written image file
static fs::path writeNoBmpFile(fs::path aFilePath, const std::vector<uint8_t>& aRawImgData)
{
    // Discard the header for non BMP images. After that, they start
    // with a complete image, e.g. PNG or JPG
    const size_t startOffset = 0xbb;

    if(startOffset >= aRawImgData.size())
    {
        throw std::runtime_error("Invalid function usage");
    }

    struct ImageFileType
    {
        std::string name;
        std::string extension;
        std::vector<uint8_t> magicBytes;
    };

    // Extension will be evaluated in this order. This is relevant
    // for magic bytes like e.g. {0xaa, 0xbb} vs {0xaa}. There we
    // want to evaluate for {0xaa, 0xbb} first.
    const std::vector<ImageFileType> fileTypes = {
        { "JPG", ".jpg", {0xff, 0xd8}             },
        { "PNG", ".png", {0x89, 0x50, 0x4e, 0x47} }
    };

    std::optional<ImageFileType> foundFileType;

    for(const auto& fileType : fileTypes)
    {
        size_t minCmpLen = std::min(fileType.magicBytes.size(), aRawImgData.size() - startOffset);

        if(minCmpLen < fileType.magicBytes.size())
        {
            continue;
        }

        bool isType = 0 == std::memcmp(fileType.magicBytes.data(), aRawImgData.data() + startOffset,
            minCmpLen);

        if(isType)
        {
            foundFileType = fileType;
            break;
        }
    }

    if(foundFileType.has_value())
    {
        aFilePath.replace_extension(foundFileType.value().extension);
    }
    else
    {
        aFilePath.replace_extension(".unknown");
        spdlog::warn("Unknown image file format");
    }

    std::ofstream img{aFilePath, std::ios::out | std::ios::binary};

    if(!img)
    {
        const std::string msg = fmt::format("{}: Can not open file for writing: {}",
            __func__, aFilePath.string());

        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    img.write(reinterpret_cast<const char*>(aRawImgData.data() + startOffset), aRawImgData.size() - startOffset);

    img.close();

    return aFilePath;
}

// The images are not always bitmaps where we need to prepend its header
// but its also possible that they are PNG, JPG and probably other formats
// that have some header that needs to be trimmed away.
static bool isBmpImage(const std::vector<uint8_t>& aRawImgData)
{
    bool hasMagicId = false;

    // Non BMP images contain a header that seems to be the same everywhere
    // one characteristic section contains the string `CI_IMAGE` that is
    // used for recognizing this kind of headers.
    const size_t MAGIC_ID_OFFSET = 0xaa;
    const std::array<uint8_t, 8> MAGIC_ID = {'C', 'I', '_', 'I', 'M', 'A', 'G', 'E'};

    if(aRawImgData.size() >= 0xbb)
    {
        hasMagicId = 0 == std::memcmp(MAGIC_ID.data(), aRawImgData.data() + MAGIC_ID_OFFSET,
            std::min(MAGIC_ID.size(), aRawImgData.size() - MAGIC_ID_OFFSET));
    }

    return !hasMagicId;
}


fs::path PrimBitmap::writeImgToFile(fs::path aFilePath) const
{
    // @todo Probably its file format version dependent if it supports
    //       only bitmaps where we need to prepend its header or supports
    //       various formats where a longer header is present that we need
    //       to discard. It would be nice seeing a bitmap image that is
    //       provided with the long header to support this theory. Then
    //       we can simplify this logic by just checking the file format version.
    if(isBmpImage(rawImgData))
    {
        spdlog::info("{}: Detected BMP file", __func__);

        aFilePath = writeBmpFile(aFilePath, rawImgData);
    }
    else
    {
        spdlog::info("{}: Detected some non BMP file", __func__);

        aFilePath = writeNoBmpFile(aFilePath, rawImgData);
    }

    return aFilePath;
}