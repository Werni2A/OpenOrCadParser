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
static std::string to_string(const SymbolsLibrary& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "introduction = " + aObj.introduction + newLine();
    str += indent(1) + "createDate   = " + DateTimeToStr(aObj.createDate) + newLine();
    str += indent(1) + "modifyDate   = " + DateTimeToStr(aObj.modifyDate) + newLine();

    str += indent(1) + "textFonts:" + newLine();
    for(size_t i = 0u; i < aObj.textFonts.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.textFonts[i]), 2);
    }

    str += indent(1) + "strLstPartField:" + newLine();
    for(size_t i = 0u; i < aObj.strLstPartField.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + aObj.strLstPartField[i], 2) + newLine();
    }

    str += indent(1) + "strLst:" + newLine();
    for(size_t i = 0u; i < aObj.strLst.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + aObj.strLst[i], 2) + newLine();
    }

    str += indent(1) + "partAliases:" + newLine();
    for(size_t i = 0u; i < aObj.partAliases.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + aObj.partAliases[i].first + " = " + aObj.partAliases[i].second, 2) + newLine();
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const SymbolsLibrary& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // SYMBOLSLIBRARY_H