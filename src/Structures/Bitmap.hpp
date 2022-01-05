#ifndef BITMAP_H
#define BITMAP_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "../General.hpp"


// @todo update Kaitai file
struct Bitmap
{
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

    // @todo Add addition header info such that the bitmap can be opened with a standard
    //       image viewer.
    void writeBmpToFile(const std::string& aFilePath) const
    {
        std::ofstream bmp(aFilePath, std::ios::out | std::ios::binary);

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
};


[[maybe_unused]]
static std::string to_string(const Bitmap& bitmap)
{
    std::string str;

    str += "Bitmap:" + newLine();
    str += indent(1) + "locX = " + std::to_string(bitmap.locX) + newLine();
    str += indent(1) + "locY = " + std::to_string(bitmap.locY) + newLine();
    str += indent(1) + "x1   = " + std::to_string(bitmap.x1)   + newLine();
    str += indent(1) + "y1   = " + std::to_string(bitmap.y1)   + newLine();
    str += indent(1) + "x2   = " + std::to_string(bitmap.x2)   + newLine();
    str += indent(1) + "y2   = " + std::to_string(bitmap.y2)   + newLine();
    str += indent(1) + "bmpWidth  = " + std::to_string(bitmap.bmpWidth)  + newLine();
    str += indent(1) + "bmpHeight = " + std::to_string(bitmap.bmpHeight) + newLine();

    // @todo Should we print rawImgData somehow?

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Bitmap& bitmap)
{
    os << to_string(bitmap);

    return os;
}


#endif // BITMAP_H