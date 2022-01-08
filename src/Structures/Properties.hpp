#ifndef PROPERTIES_H
#define PROPERTIES_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>


struct Properties
{
    std::string name;
    std::string ref;

    std::string convertName;
};


[[maybe_unused]]
static std::string to_string(const Properties& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "name = " + aObj.name + newLine();
    str += indent(1) + "ref  = " + aObj.ref  + newLine();
    str += indent(1) + "convertName = " + aObj.convertName + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Properties& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PROPERTIES_H