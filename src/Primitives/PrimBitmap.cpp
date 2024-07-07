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
#include "GenericParser.hpp"
#include "Primitives/PrimBitmap.hpp"

namespace fs = std::filesystem;

void OOCP::PrimBitmap::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const size_t startOffset = ds.getCurrentOffset();

    const uint32_t byteLength = ds.readUint32();

    ds.assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    locX = ds.readInt32();
    locY = ds.readInt32();

    mCtx.mLogger.trace("locX = {}", locX);
    mCtx.mLogger.trace("locY = {}", locY);

    x2 = ds.readInt32();
    y2 = ds.readInt32();
    x1 = ds.readInt32();
    y1 = ds.readInt32();

    mCtx.mLogger.trace("x2 = {}", x2);
    mCtx.mLogger.trace("y2 = {}", y2);
    mCtx.mLogger.trace("x1 = {}", x1);
    mCtx.mLogger.trace("y1 = {}", y1);

    bmpWidth  = ds.readUint32();
    bmpHeight = ds.readUint32();

    mCtx.mLogger.trace("bmpWidth  = {}", bmpWidth);
    mCtx.mLogger.trace("bmpHeight = {}", bmpHeight);

    const uint32_t dataSize = ds.readUint32();

    mCtx.mLogger.trace("dataSize = {}", dataSize);

    rawImgData.clear();
    rawImgData.reserve(dataSize);

    for(size_t i = 0U; i < dataSize; ++i)
    {
        rawImgData.push_back(ds.readUint8());
    }

    fs::path filename = mCtx.mExtractedCfbfPath.parent_path() / "data" /
                        fmt::format("{}_img_{}.bmp", mCtx.mInputStream.stem().string(), mCtx.mImgCtr);

    filename = writeImgToFile(filename);

    mCtx.mLogger.info("{}: Wrote bitmap file to {}", __func__, filename.string());

    ++mCtx.mImgCtr;

    if(ds.getCurrentOffset() != startOffset + byteLength)
    {
        // throw MisinterpretedData(__func__, startOffset, byteLength, ds.getCurrentOffset());
    }

    if(byteLength != 44U + dataSize)
    {
        // throw FileFormatChanged("Bitmap");
    }

    parser.readPreamble();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}

// Returns path to the written image file
fs::path OOCP::PrimBitmap::writeBmpFile(fs::path aFilePath, const std::vector<uint8_t>& aRawImgData) const
{
    aFilePath.replace_extension(".bmp");

    fs::create_directories(aFilePath.parent_path());
    std::ofstream img{aFilePath, std::ios::out | std::ios::binary};

    if(!img)
    {
        const std::string msg = fmt::format("{}: Can not open file for writing: {}", __func__, aFilePath.string());

        mCtx.mLogger.error(msg);
        throw std::runtime_error(msg);
    }

    // Reconstruct header information that was not stored in the file container
    // See https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header
    // for further details on the meaning of each configuration value.
    struct BmpHeader
    {
        uint16_t magicBytes = 0x4d42;
        uint32_t bmpSize    = 0; // Will be set later
        uint32_t reserved   = 0;
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

    mCtx.mLogger.debug("Writing header is done");

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
fs::path OOCP::PrimBitmap::writeDifferentImageFile(fs::path aFilePath, const std::vector<uint8_t>& aRawImgData) const
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
        {"JPG", ".jpg", {0xff, 0xd8}            },
        {"PNG", ".png", {0x89, 0x50, 0x4e, 0x47}}
    };

    std::optional<ImageFileType> foundFileType;

    for(const auto& fileType : fileTypes)
    {
        size_t minCmpLen = std::min(fileType.magicBytes.size(), aRawImgData.size() - startOffset);

        if(minCmpLen < fileType.magicBytes.size())
        {
            continue;
        }

        bool isType = 0 == std::memcmp(fileType.magicBytes.data(), aRawImgData.data() + startOffset, minCmpLen);

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
        mCtx.mLogger.warn("Unknown image file format");
    }

    fs::create_directories(aFilePath.parent_path());
    std::ofstream img{aFilePath, std::ios::out | std::ios::binary};

    if(!img)
    {
        const std::string msg = fmt::format("{}: Can not open file for writing: {}", __func__, aFilePath.string());

        mCtx.mLogger.error(msg);
        throw std::runtime_error(msg);
    }

    img.write(reinterpret_cast<const char*>(aRawImgData.data() + startOffset), aRawImgData.size() - startOffset);

    img.close();

    return aFilePath;
}

// The images are not always bitmaps where we need to prepend its header
// but its also possible that they are PNG, JPG and probably other formats
// that have some header that needs to be trimmed away.
bool OOCP::PrimBitmap::isBmpImage(const std::vector<uint8_t>& aRawImgData) const
{
    bool hasMagicId = false;

    // Non BMP images contain a header that seems to be the same everywhere
    // one characteristic section contains the string `CI_IMAGE` that is
    // used for recognizing this kind of headers.
    const size_t MAGIC_ID_OFFSET          = 0xaa;
    const std::array<uint8_t, 8> MAGIC_ID = {'C', 'I', '_', 'I', 'M', 'A', 'G', 'E'};

    if(aRawImgData.size() >= 0xbb)
    {
        hasMagicId = 0 == std::memcmp(MAGIC_ID.data(), aRawImgData.data() + MAGIC_ID_OFFSET,
                              std::min(MAGIC_ID.size(), aRawImgData.size() - MAGIC_ID_OFFSET));
    }

    return !hasMagicId;
}

fs::path OOCP::PrimBitmap::writeImgToFile(fs::path aFilePath) const
{
    // @todo Probably its file format version dependent if it supports
    //       only bitmaps where we need to prepend its header or supports
    //       various formats where a longer header is present that we need
    //       to discard. It would be nice seeing a bitmap image that is
    //       provided with the long header to support this theory. Then
    //       we can simplify this logic by just checking the file format version.
    if(isBmpImage(rawImgData))
    {
        mCtx.mLogger.info("{}: Detected BMP file", getMethodName(this, __func__));

        aFilePath = writeBmpFile(aFilePath, rawImgData);
    }
    else
    {
        mCtx.mLogger.info("{}: Detected some non BMP file", getMethodName(this, __func__));

        aFilePath = writeDifferentImageFile(aFilePath, rawImgData);
    }

    return aFilePath;
}