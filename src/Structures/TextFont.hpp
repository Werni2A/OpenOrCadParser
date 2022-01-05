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
static std::string to_string(const TextFont& textFont)
{
    std::string str;

    str += "TextFont:" + newLine();
    str += indent(1) + "height = " + std::to_string(textFont.height) + newLine();
    str += indent(1) + "width  = " + std::to_string(textFont.width)  + newLine();
    str += indent(1) + "escapement = " + std::to_string(textFont.escapement) + newLine();
    str += indent(1) + "weight = " + std::to_string(textFont.weight) + newLine();
    str += indent(1) + "italic = " + std::to_string(textFont.italic) + newLine();
    str += indent(1) + "fontName = " + textFont.fontName + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const TextFont& textFont)
{
    os << to_string(textFont);

    return os;
}


#endif // TEXTFONT_H