#ifndef SYMBOLDISPLAYPROP_H
#define SYMBOLDISPLAYPROP_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

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

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "nameIdx     = " + std::to_string(aObj.nameIdx) + newLine();
    str += indent(1) + "textFontIdx = " + std::to_string(aObj.textFontIdx) + newLine();
    str += indent(1) + "rotation    = " + to_string(aObj.rotation) + newLine();
    str += indent(1) + "x = " + std::to_string(aObj.x) + newLine();
    str += indent(1) + "y = " + std::to_string(aObj.y) + newLine();
    str += indent(1) + "propColor = " + to_string(aObj.propColor) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const SymbolDisplayProp& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // SYMBOLDISPLAYPROP_H