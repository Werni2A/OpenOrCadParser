#ifndef SYMBOLSLIBRARY_H
#define SYMBOLSLIBRARY_H


#include <cstdint>
#include <ctime>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "TextFont.hpp"


struct SymbolsLibrary
{
    std::string introduction;

    time_t createDate;
    time_t modifyDate;

    std::vector<TextFont> textFonts;

    std::vector<std::string> strLstPartField;
    std::vector<std::string> strLst;

    // See OrCAD: 'Package Properties' -> 'Part Aliases'
    std::vector<std::pair<std::string, std::string>> partAliases; //!< .first = Alias, .second = Package
};


[[maybe_unused]]
static std::string to_string(const SymbolsLibrary& symbolsLibrary)
{
    std::string str;

    str += "SymbolsLibrary:" + newLine();
    str += indent(1) + "introduction = " + symbolsLibrary.introduction + newLine();
    str += indent(1) + "createDate   = " + DateTimeToStr(symbolsLibrary.createDate) + newLine();
    str += indent(1) + "modifyDate   = " + DateTimeToStr(symbolsLibrary.modifyDate) + newLine();

    str += indent(1) + "textFonts:" + newLine();
    for(size_t i = 0u; i < symbolsLibrary.textFonts.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(symbolsLibrary.textFonts[i]), 2);
    }

    str += indent(1) + "strLstPartField:" + newLine();
    for(size_t i = 0u; i < symbolsLibrary.strLstPartField.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + symbolsLibrary.strLstPartField[i], 2) + newLine();
    }

    str += indent(1) + "strLst:" + newLine();
    for(size_t i = 0u; i < symbolsLibrary.strLst.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + symbolsLibrary.strLst[i], 2) + newLine();
    }

    str += indent(1) + "partAliases:" + newLine();
    for(size_t i = 0u; i < symbolsLibrary.partAliases.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + symbolsLibrary.partAliases[i].first + " = " + symbolsLibrary.partAliases[i].second, 2) + newLine();
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const SymbolsLibrary& symbolsLibrary)
{
    os << to_string(symbolsLibrary);
    return os;
}


#endif // SYMBOLSLIBRARY_H