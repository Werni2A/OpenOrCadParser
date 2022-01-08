#ifndef BITMAP_H
#define BITMAP_H


#include <cstdint>
#include <ostream>
#include <string>

#include "../General.hpp"


// @todo update Kaitai file
struct Bitmap
{
    void writeBmpToFile(const std::string& aFilePath) const;

    int32_t locX;
    int32_t locY;

    int32_t x1;
    int32_t y1;
    int32_t x2;
    int32_t y2;

    uint32_t bmpWidth;
    uint32_t bmpHeight;

    // @todo Add better Bitmap support and figure out how to
    //       create a *.bmp file out of the rawImgData
    // @note Looks like the XSD uses Base64 encoding for the data, but I was not
    //       able to extract the correct content from there. Are there some parameters
    //       to adjust in the decoding? https://cryptii.com/pipes/base64-to-binary
    std::vector<uint8_t> rawImgData; // @todo called val in the XSD file
};


[[maybe_unused]]
static std::string to_string(const Bitmap& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "locX = " + std::to_string(aObj.locX) + newLine();
    str += indent(1) + "locY = " + std::to_string(aObj.locY) + newLine();
    str += indent(1) + "x1   = " + std::to_string(aObj.x1)   + newLine();
    str += indent(1) + "y1   = " + std::to_string(aObj.y1)   + newLine();
    str += indent(1) + "x2   = " + std::to_string(aObj.x2)   + newLine();
    str += indent(1) + "y2   = " + std::to_string(aObj.y2)   + newLine();
    str += indent(1) + "bmpWidth  = " + std::to_string(aObj.bmpWidth)  + newLine();
    str += indent(1) + "bmpHeight = " + std::to_string(aObj.bmpHeight) + newLine();

    // @todo Should we print rawImgData somehow?

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Bitmap& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // BITMAP_H