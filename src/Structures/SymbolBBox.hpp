#ifndef SYMBOLBBOX_H
#define SYMBOLBBOX_H


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../General.hpp"


struct SymbolBBox
{
    // @todo they are probably int16_t, see where those values are read.
    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;
};


[[maybe_unused]]
static std::string to_string(const SymbolBBox& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}x1 = {}\n", indent(1), aObj.x1);
    str += fmt::format("{}y1 = {}\n", indent(1), aObj.y1);
    str += fmt::format("{}x2 = {}\n", indent(1), aObj.x2);
    str += fmt::format("{}y2 = {}\n", indent(1), aObj.y2);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const SymbolBBox& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // SYMBOLBBOX_H