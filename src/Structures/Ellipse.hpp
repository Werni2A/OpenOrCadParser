



#ifndef ELLIPSE_H
#define ELLIPSE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

#include "../Enums/HatchStyle.hpp"
#include "../Enums/FillStyle.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"


struct Ellipse
{
    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;

    LineStyle lineStyle;
    LineWidth lineWidth;
    FillStyle fillStyle;
    HatchStyle hatchStyle;
};


[[maybe_unused]]
static std::string to_string(const Ellipse& ellipse)
{
    std::string str;

    str += "Ellipse:" + newLine();
    str += indent(1) + "x1 = " + std::to_string(ellipse.x1) + newLine();
    str += indent(1) + "y1 = " + std::to_string(ellipse.y1) + newLine();
    str += indent(1) + "x2 = " + std::to_string(ellipse.x2) + newLine();
    str += indent(1) + "y2 = " + std::to_string(ellipse.y2) + newLine();
    str += indent(1) + "lineStyle  = " + to_string(ellipse.lineStyle) + newLine();
    str += indent(1) + "lineWidth  = " + to_string(ellipse.lineWidth) + newLine();
    str += indent(1) + "fillStyle  = " + to_string(ellipse.fillStyle) + newLine();

    if(ellipse.fillStyle == FillStyle::HatchPattern)
    {
        str += indent(1) + "hatchStyle = " + to_string(ellipse.hatchStyle) + newLine();
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Ellipse& ellipse)
{
    os << to_string(ellipse);

    return os;
}


#endif // ELLIPSE_H