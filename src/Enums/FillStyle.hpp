#ifndef FILLSTYLE_H
#define FILLSTYLE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


// This configuration dominates the hatch_style.
// I.e. when solid/none is set, the hatch_style
// value is ignored.
enum class FillStyle
{
    Solid        = 0,
    None         = 1,
    HatchPattern = 2  // Refer to HatchStyle
};


[[maybe_unused]]
static FillStyle ToFillStyle(uint32_t val)
{
    FillStyle fillStyle;

    switch(val)
    {
        case 0: fillStyle = FillStyle::Solid;        break;
        case 1: fillStyle = FillStyle::None;         break;
        case 2: fillStyle = FillStyle::HatchPattern; break;
        default:
            std::string errorMsg = "FillStyle with value " + std::to_string(val)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return fillStyle;
}


[[maybe_unused]]
static std::string to_string(const FillStyle& fillStyle)
{
    std::string str;

    switch(fillStyle)
    {
        case FillStyle::Solid:        str = "Solid";        break;
        case FillStyle::None:         str = "None";         break;
        case FillStyle::HatchPattern: str = "HatchPattern"; break;
        default:
            std::string errorMsg = "FillStyle " + std::to_string(static_cast<size_t>(fillStyle))
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const FillStyle& fillStyle)
{
    os << to_string(fillStyle) << std::endl;
    return os;
}


#endif // FILLSTYLE_H