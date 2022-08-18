#ifndef COORDINATE_H
#define COORDINATE_H


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../General.hpp"


/// @note Represents 'BezierPoint', 'PolygonPoint' and 'PolylinePoint' because
///       all of them use the same point structure.
struct Point
{
    uint16_t x;
    uint16_t y;
};


[[maybe_unused]]
static std::string to_string(const Point& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}x = {}\n", indent(1), aObj.x);
    str += fmt::format("{}y = {}\n", indent(1), aObj.y);
    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Point& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // COORDINATE_H