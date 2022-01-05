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
static std::string to_string(const Point& point)
{
    std::string str;

    str += "Point:" + newLine();
    str += indent(1) + "x = " + std::to_string(point.x) + newLine();
    str += indent(1) + "y = " + std::to_string(point.y) + newLine();
    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Point& point)
{
    os << to_string(point);

    return os;
}


#endif // COORDINATE_H