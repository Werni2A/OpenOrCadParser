#ifndef RECT_H
#define RECT_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

#include "../General.hpp"
#include "../Structures/Point.hpp"
#include "../Enums/FillStyle.hpp"
#include "../Enums/HatchStyle.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"


struct Rect
{
    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;

    LineStyle lineStyle;
    LineWidth lineWidth;
    FillStyle fillStyle;
    HatchStyle hatchStyle;


    // @todo implement instead of having the functionality in parseRect()
    static constexpr uint32_t getExpectedByteLength(FileFormatVersion version)
    {
        uint32_t expectedByteLength = 0u;
        switch(version)
        {
            case FileFormatVersion::A:
                // expectedByteLength = 24u; break;
            case FileFormatVersion::B:
                expectedByteLength = 32u; break;
            case FileFormatVersion::C:
                expectedByteLength = 40u; break;
            // default:
                // @todo probably remove this check. When something changes we will see
                //       it as the expectedByteLength does not match anymore.
                //       They probably won't permute the content in the structure, won't they?
                // throw MissingFileFormatCheck(__func__, __LINE__, version); break;
        }

        return expectedByteLength;

        // @todo better represent the version as a fixed point value in unsigned representation
        //       and check for ranges. E.g. if in [0, 120) then else if [120, 250) else ...
    }


};


[[maybe_unused]]
static std::string to_string(const Rect& rect)
{
    std::string str;

    str += "Rect:" + newLine();
    str += indent(1) + "x1 = " + std::to_string(rect.x1) + newLine();
    str += indent(1) + "y1 = " + std::to_string(rect.y1) + newLine();
    str += indent(1) + "x2 = " + std::to_string(rect.x2) + newLine();
    str += indent(1) + "y2 = " + std::to_string(rect.y2) + newLine();
    str += indent(1) + "lineStyle  = " + to_string(rect.lineStyle) + newLine();
    str += indent(1) + "lineWidth  = " + to_string(rect.lineWidth) + newLine();
    str += indent(1) + "fillStyle  = " + to_string(rect.fillStyle) + newLine();

    if(rect.fillStyle == FillStyle::HatchPattern)
    {
        str += indent(1) + "hatchStyle = " + to_string(rect.hatchStyle) + newLine();
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Rect& rect)
{
    os << to_string(rect);

    return os;
}


#endif // RECT_H