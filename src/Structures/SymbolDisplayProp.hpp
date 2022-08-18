#ifndef SYMBOLDISPLAYPROP_H
#define SYMBOLDISPLAYPROP_H


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../Enums/Color.hpp"
#include "../Enums/Rotation.hpp"


struct SymbolDisplayProp
{
    uint32_t nameIdx;
    uint16_t textFontIdx;

    Rotation rotation;

    int16_t x;
    int16_t y;

    Color propColor;
};

// @todo implement getter methods for name and textFont

[[maybe_unused]]
static std::string to_string(const SymbolDisplayProp& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}nameIdx     = {}\n", indent(1), aObj.nameIdx);
    str += fmt::format("{}textFontIdx = {}\n", indent(1), aObj.textFontIdx);
    str += fmt::format("{}rotation    = {}\n", indent(1), to_string(aObj.rotation));
    str += fmt::format("{}x = {}\n", indent(1), aObj.x);
    str += fmt::format("{}y = {}\n", indent(1), aObj.y);
    str += fmt::format("{}propColor   = {}\n", indent(1), to_string(aObj.propColor));

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const SymbolDisplayProp& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // SYMBOLDISPLAYPROP_H