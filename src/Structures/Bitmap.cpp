#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include <nameof.hpp>

#include "../General.hpp"
#include "Bitmap.hpp"

#include "../Parser.hpp"


// @todo Add additional header info such that the bitmap can be opened with a standard
//       image viewer.
void Bitmap::writeBmpToFile(const std::string& aFilePath) const
{
    std::ofstream bmp{aFilePath, std::ios::out | std::ios::binary};

    if(!bmp)
    {
        std::cerr << "Cannot open file!" << std::endl;
        exit(1);
    }

    for(const auto& data : rawImgData)
    {
        bmp.write(reinterpret_cast<const char*>(&data), sizeof(data));
    }

    bmp.close();
}


Bitmap Parser::readBitmap()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Bitmap obj;

    const uint32_t byteLength = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    obj.locX = mDs.readInt32();
    obj.locY = mDs.readInt32();

    obj.x2 = mDs.readInt32();
    obj.y2 = mDs.readInt32();
    obj.x1 = mDs.readInt32();
    obj.y1 = mDs.readInt32();

    obj.bmpWidth  = mDs.readUint32();
    obj.bmpHeight = mDs.readUint32();

    const uint32_t imgSize = mDs.readUint32();

    obj.rawImgData.clear();

    for(size_t i = 0u; i < imgSize; ++i)
    {
        obj.rawImgData.push_back(mDs.readUint8());
    }

    // obj.writeBmpToFile("foo" + std::to_string(imgSize) + ".bmp"); // @todo Require useful name

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != 44u + imgSize)
    {
        throw FileFormatChanged("Bitmap");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << obj << std::endl;

    return obj;
}