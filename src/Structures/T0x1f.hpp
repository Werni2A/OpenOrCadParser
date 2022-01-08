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
static std::string to_string(const T0x1f& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "name   = " + aObj.name   + newLine();
    str += indent(1) + "refDes = " + aObj.refDes + newLine();
    str += indent(1) + "pcbFootprint = " + aObj.pcbFootprint + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const T0x1f& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // T0X1F_H