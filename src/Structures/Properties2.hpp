#ifndef PROPERTIES2_H
#define PROPERTIES2_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>


struct Properties2
{
    std::string name;
    std::string refDes;
    std::string footprint;

    uint16_t sectionCount;
};


[[maybe_unused]]
static std::string to_string(const Properties2& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "name   = " + aObj.name   + newLine();
    str += indent(1) + "refDes = " + aObj.refDes + newLine();
    str += indent(1) + "footprint    = " + aObj.footprint + newLine();
    str += indent(1) + "sectionCount = " + std::to_string(aObj.sectionCount) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Properties2& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PROPERTIES2_H