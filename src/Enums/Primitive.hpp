#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"


enum class Primitive
{
    Rect         = 40,
    Line         = 41,
    Arc          = 42,
    Ellipse      = 43,
    Polygon      = 44,
    Polyline     = 45,
    CommentText  = 46,
    Bitmap       = 47,
    SymbolVector = 48,
    Bezier       = 87
};


[[maybe_unused]]
static constexpr Primitive ToPrimitive(uint8_t aVal)
{
    return ToEnum<Primitive, decltype(aVal)>(aVal);
}


[[maybe_unused]]
static std::string to_string(const Primitive& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Primitive& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // PRIMITIVE_HPP