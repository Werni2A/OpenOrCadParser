#ifndef GEOMETRYSTRUCTURE_H
#define GEOMETRYSTRUCTURE_H


#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "../General.hpp"


// @todo Rename to something like Graphics this would make more sense
enum class GeometryStructure
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
static constexpr GeometryStructure ToGeometryStructure(uint8_t aVal)
{
    return ToEnum<GeometryStructure, decltype(aVal)>(aVal);
}


[[maybe_unused]]
static std::string to_string(const GeometryStructure& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const GeometryStructure& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // GEOMETRYSTRUCTURE_H