#ifndef TEXTFONT_H
#define TEXTFONT_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "../General.hpp"


struct TextFont
{
    int32_t height;
    int32_t width;

    uint16_t escapement;

    uint16_t weight;

    uint16_t italic;

    std::string fontName;
};


[[maybe_unused]]
static std::string to_string(const TextFont& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "height = " + std::to_string(aObj.height) + newLine();
    str += indent(1) + "width  = " + std::to_string(aObj.width)  + newLine();
    str += indent(1) + "escapement = " + std::to_string(aObj.escapement) + newLine();
    str += indent(1) + "weight = " + std::to_string(aObj.weight) + newLine();
    str += indent(1) + "italic = " + std::to_string(aObj.italic) + newLine();
    str += indent(1) + "fontName = " + aObj.fontName + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const TextFont& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // TEXTFONT_H