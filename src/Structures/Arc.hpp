#ifndef ARC_H
#define ARC_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"


// @note This is actually an elliptic arc.
struct Arc
{
    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;

    int32_t startX;
    int32_t startY;

    int32_t endX;
    int32_t endY;

    LineStyle lineStyle;
    LineWidth lineWidth;
};


[[maybe_unused]]
static std::string to_string(const Arc& arc)
{
    std::string str;

    str += "Arc:" + newLine();
    str += indent(1) + "x1 = " + std::to_string(arc.x1) + newLine();
    str += indent(1) + "y1 = " + std::to_string(arc.y1) + newLine();
    str += indent(1) + "x2 = " + std::to_string(arc.x2) + newLine();
    str += indent(1) + "y2 = " + std::to_string(arc.y2) + newLine();
    str += indent(1) + "startX = " + std::to_string(arc.startX)  + newLine();
    str += indent(1) + "startY = " + std::to_string(arc.startY)  + newLine();
    str += indent(1) + "endX   = " + std::to_string(arc.endX)    + newLine();
    str += indent(1) + "endY   = " + std::to_string(arc.endY)    + newLine();
    str += indent(1) + "lineStyle = " + to_string(arc.lineStyle) + newLine();
    str += indent(1) + "lineWidth = " + to_string(arc.lineWidth) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Arc& arc)
{
    os << to_string(arc);

    return os;
}


#endif // ARC_H