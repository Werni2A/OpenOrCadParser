#ifndef POLYGON_H
#define POLYGON_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "../Structures/Point.hpp"
#include "../Enums/FillStyle.hpp"
#include "../Enums/HatchStyle.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"


struct Polygon
{
    LineStyle lineStyle;
    LineWidth lineWidth;
    FillStyle fillStyle;
    HatchStyle hatchStyle;

    std::vector<Point> points;

    std::string to_string() const;
};


// @todo this looks like a better solution for to_string. Implement for all other
//       structures and remove operator<<.
// std::string Polygon::to_string() const
// {
//     std::string str;

//     str += "Polygon:" + newLine();
//     str += indent(1) + "lineStyle  = " + ::to_string(lineStyle) + newLine();
//     str += indent(1) + "lineWidth  = " + ::to_string(lineWidth) + newLine();
//     str += indent(1) + "fillStyle  = " + ::to_string(fillStyle) + newLine();

//     if(fillStyle == FillStyle::HatchPattern)
//     {
//         str += indent(1) + "hatchStyle = " + ::to_string(hatchStyle) + newLine();
//     }

//     str += indent(1) + "points:" + newLine();
//     for(size_t i = 0u; i < points.size(); ++i)
//     {
//         str += indent(std::to_string(i) + ": " + ::to_string(points[i]), 2);
//     }

//     return str;
// }


[[maybe_unused]]
static std::string to_string(const Polygon& polygon)
{
    std::string str;

    str += "Polygon:" + newLine();
    str += indent(1) + "lineStyle  = " + to_string(polygon.lineStyle) + newLine();
    str += indent(1) + "lineWidth  = " + to_string(polygon.lineWidth) + newLine();
    str += indent(1) + "fillStyle  = " + to_string(polygon.fillStyle) + newLine();

    if(polygon.fillStyle == FillStyle::HatchPattern)
    {
        str += indent(1) + "hatchStyle = " + to_string(polygon.hatchStyle) + newLine();
    }

    str += indent(1) + "points:" + newLine();
    for(size_t i = 0u; i < polygon.points.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(polygon.points[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Polygon& polygon)
{
    os << to_string(polygon);
    return os;
}


#endif // POLYGON_H