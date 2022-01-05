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
static std::string to_string(const SymbolDisplayProp& symbolDisplayProp)
{
    std::string str;

    str += "SymbolDisplayProp:" + newLine();
    str += indent(1) + "nameIdx = " + std::to_string(symbolDisplayProp.nameIdx) + newLine();
    str += indent(1) + "textFontIdx = " + std::to_string(symbolDisplayProp.textFontIdx) + newLine();
    str += indent(1) + "rotation = " + to_string(symbolDisplayProp.rotation) + newLine();
    str += indent(1) + "x = " + std::to_string(symbolDisplayProp.x) + newLine();
    str += indent(1) + "y = " + std::to_string(symbolDisplayProp.y) + newLine();
    str += indent(1) + "propColor = " + to_string(symbolDisplayProp.propColor) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const SymbolDisplayProp& symbolDisplayProp)
{
    os << to_string(symbolDisplayProp);

    return os;
}


#endif // SYMBOLDISPLAYPROP_H