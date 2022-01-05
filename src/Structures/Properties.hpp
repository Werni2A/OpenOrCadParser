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
static std::string to_string(const Properties& properties)
{
    std::string str;

    str += "Properties:" + newLine();
    str += indent(1) + "name = " + properties.name + newLine();
    str += indent(1) + "ref  = " + properties.ref  + newLine();
    str += indent(1) + "convertName = " + properties.convertName + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Properties& properties)
{
    os << to_string(properties);

    return os;
}


#endif // PROPERTIES_H