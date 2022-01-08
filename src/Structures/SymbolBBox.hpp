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
static std::string to_string(const SymbolBBox& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "x1 = " + std::to_string(aObj.x1) + newLine();
    str += indent(1) + "y1 = " + std::to_string(aObj.y1) + newLine();
    str += indent(1) + "x2 = " + std::to_string(aObj.x2) + newLine();
    str += indent(1) + "y2 = " + std::to_string(aObj.y2) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const SymbolBBox& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // SYMBOLBBOX_H