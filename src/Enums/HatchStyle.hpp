#ifndef HATCHSTYLE_H
#define HATCHSTYLE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


enum class HatchStyle
{
    LinesHorizontal = 0, // -
    LinesVertical   = 1, // |
    DiagonalLeft    = 2, // \'
    DiagonalRight   = 3, // /
    Checkerboard    = 4, // +
    Mesh            = 5  // x
};


[[maybe_unused]]
static HatchStyle ToHatchStyle(uint32_t val)
{
    HatchStyle hatchStyle;

    switch(val)
    {
        case 0: hatchStyle = HatchStyle::LinesHorizontal; break;
        case 1: hatchStyle = HatchStyle::LinesVertical;   break;
        case 2: hatchStyle = HatchStyle::DiagonalLeft;    break;
        case 3: hatchStyle = HatchStyle::DiagonalRight;   break;
        case 4: hatchStyle = HatchStyle::Checkerboard;    break;
        case 5: hatchStyle = HatchStyle::Mesh;            break;
        default:
            std::string errorMsg = "HatchStyle with value " + std::to_string(val)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return hatchStyle;
}


[[maybe_unused]]
static std::string to_string(const HatchStyle& hatchStyle)
{
    std::string str;

    switch(hatchStyle)
    {
        case HatchStyle::LinesHorizontal: str = "LinesHorizontal"; break;
        case HatchStyle::LinesVertical:   str = "LinesVertical";   break;
        case HatchStyle::DiagonalLeft:    str = "DiagonalLeft";    break;
        case HatchStyle::DiagonalRight:   str = "DiagonalRight";   break;
        case HatchStyle::Checkerboard:    str = "Checkerboard";    break;
        case HatchStyle::Mesh:            str = "Mesh";            break;
        default:
            std::string errorMsg = "HatchStyle " + std::to_string(static_cast<size_t>(hatchStyle))
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const HatchStyle& hatchStyle)
{
    os << to_string(hatchStyle);
    return os;
}


#endif // HATCHSTYLE_H