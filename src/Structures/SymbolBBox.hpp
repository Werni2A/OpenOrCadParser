#ifndef SYMBOLBBOX_H
#define SYMBOLBBOX_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

#include "../General.hpp"


struct SymbolBBox
{
    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;
};


[[maybe_unused]]
static std::string to_string(const SymbolBBox& symbolBBox)
{
    std::string str;

    str += "SymbolBBox:" + newLine();
    str += indent(1) + "x1 = " + std::to_string(symbolBBox.x1) + newLine();
    str += indent(1) + "y1 = " + std::to_string(symbolBBox.y1) + newLine();
    str += indent(1) + "x2 = " + std::to_string(symbolBBox.x2) + newLine();
    str += indent(1) + "y2 = " + std::to_string(symbolBBox.y2) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const SymbolBBox& symbolBBox)
{
    os << to_string(symbolBBox);

    return os;
}


#endif // SYMBOLBBOX_H