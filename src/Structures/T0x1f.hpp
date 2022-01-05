#ifndef T0X1F_H
#define T0X1F_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>


struct T0x1f
{
    std::string name;
    std::string refDes;
    std::string pcbFootprint;
};


[[maybe_unused]]
static std::string to_string(const T0x1f& t0x1f)
{
    std::string str;

    str += "T0x1f:" + newLine();
    str += indent(1) + "name   = " + t0x1f.name   + newLine();
    str += indent(1) + "refDes = " + t0x1f.refDes + newLine();
    str += indent(1) + "pcbFootprint = " + t0x1f.pcbFootprint + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const T0x1f& t0x1f)
{
    os << to_string(t0x1f);

    return os;
}


#endif // T0X1F_H