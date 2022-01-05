#ifndef LINE_H
#define LINE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"


struct Line
{
    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;

    LineStyle lineStyle;
    LineWidth lineWidth;
};


[[maybe_unused]]
static std::string to_string(const Line& line)
{
    std::string str;

    str += "Line:" + newLine();
    str += indent(1) + "x1 = " + std::to_string(line.x1) + newLine();
    str += indent(1) + "y1 = " + std::to_string(line.y1) + newLine();
    str += indent(1) + "x2 = " + std::to_string(line.x2) + newLine();
    str += indent(1) + "y2 = " + std::to_string(line.y2) + newLine();
    str += indent(1) + "lineStyle = " + to_string(line.lineStyle) + newLine();
    str += indent(1) + "lineWidth = " + to_string(line.lineWidth) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Line& line)
{
    os << to_string(line);

    return os;
}


#endif // LINE_H