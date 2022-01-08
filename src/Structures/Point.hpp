#ifndef COORDINATE_H
#define COORDINATE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

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

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "x = " + std::to_string(aObj.x) + newLine();
    str += indent(1) + "y = " + std::to_string(aObj.y) + newLine();
    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Point& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // COORDINATE_H