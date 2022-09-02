#ifndef LINESTYLE_HPP
#define LINESTYLE_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"


enum class LineStyle
{
    Solid      = 0, // ______
    Dash       = 1, // ------
    Dot        = 2, // ......
    DashDot    = 3, // -.-.-.
    DashDotDot = 4, // -..-..
    Default    = 5
};


[[maybe_unused]]
static constexpr LineStyle ToLineStyle(uint32_t aVal)
{
    return ToEnum<LineStyle, decltype(aVal)>(aVal);
}


[[maybe_unused]]
static std::string to_string(const LineStyle& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const LineStyle& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // LINESTYLE_HPP