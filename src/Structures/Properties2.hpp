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
static std::string to_string(const Properties2& properties2)
{
    std::string str;

    str += "Properties2:" + newLine();
    str += indent(1) + "name   = " + properties2.name   + newLine();
    str += indent(1) + "refDes = " + properties2.refDes + newLine();
    str += indent(1) + "footprint    = " + properties2.footprint + newLine();
    str += indent(1) + "sectionCount = " + std::to_string(properties2.sectionCount) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Properties2& properties2)
{
    os << to_string(properties2);

    return os;
}


#endif // PROPERTIES2_H