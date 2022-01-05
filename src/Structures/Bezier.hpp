



#ifndef BEZIER_H
#define BEZIER_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "../Structures/Point.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"


struct Bezier
{
    LineStyle lineStyle;
    LineWidth lineWidth;

    std::vector<Point> points;
};


[[maybe_unused]]
static std::string to_string(const Bezier& bezier)
{
    std::string str;

    str += "Bezier:" + newLine();
    str += indent(1) + "lineStyle  = " + to_string(bezier.lineStyle) + newLine();;
    str += indent(1) + "lineWidth  = " + to_string(bezier.lineWidth) + newLine();;

    str += indent(1) + "points:" + newLine();
    for(size_t i = 0u; i < bezier.points.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(bezier.points[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Bezier& bezier)
{
    os << to_string(bezier);

    return os;
}


#endif // BEZIER_H