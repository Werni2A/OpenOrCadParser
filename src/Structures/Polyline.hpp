#ifndef POLYLINE_H
#define POLYLINE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "../Structures/Point.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"


struct Polyline
{
    LineStyle lineStyle;
    LineWidth lineWidth;

    std::vector<Point> points;
};


[[maybe_unused]]
static std::string to_string(const Polyline& polyline)
{
    std::string str;

    str += "Polyline:" + newLine();
    str += indent(1) + "lineStyle = " + to_string(polyline.lineStyle) + newLine();
    str += indent(1) + "lineWidth = " + to_string(polyline.lineWidth) + newLine();

    str += indent(1) + "points:" + newLine();
    for(size_t i = 0u; i < polyline.points.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(polyline.points[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Polyline& polyline)
{
    os << to_string(polyline);
    return os;
}


#endif // POLYLINE_H