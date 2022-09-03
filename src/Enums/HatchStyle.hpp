#ifndef HATCHSTYLE_HPP
#define HATCHSTYLE_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"


enum class HatchStyle
{
    // @todo NotValid is probably a new value in later OrCad versions. I think
    //       previous versions just ignore the value set here but it could be any value
    NotValid        = -1, // When FillStyle != HatchPattern @todo could also mean Solid! Verify this
    LinesHorizontal =  0, // -
    LinesVertical   =  1, // |
    DiagonalLeft    =  2, // \'
    DiagonalRight   =  3, // /
    Checkerboard    =  4, // +
    Mesh            =  5  // x
};


[[maybe_unused]]
static constexpr HatchStyle ToHatchStyle(int32_t aVal)
{
    return ToEnum<HatchStyle, decltype(aVal)>(aVal);
}


[[maybe_unused]]
static std::string to_string(const HatchStyle& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const HatchStyle& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // HATCHSTYLE_HPP