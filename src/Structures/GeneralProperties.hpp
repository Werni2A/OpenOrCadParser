#ifndef GENERALPROPERTIES_H
#define GENERALPROPERTIES_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "../Enums/ImplementationType.hpp"


struct GeneralProperties
{
    std::string implementationPath;
    std::string implementation;
    std::string refDes;
    std::string partValue;

    ImplementationType implementationType;

    bool pinNameVisible;
    bool pinNameRotate;
    bool pinNumberVisible;
};


[[maybe_unused]]
static std::string to_string(const GeneralProperties& generalProperties)
{
    std::string str;

    str += "GeneralProperties:" + newLine();
    str += indent(1) + "implementationPath = " + generalProperties.implementationPath + newLine();
    str += indent(1) + "implementation     = " + generalProperties.implementation     + newLine();
    str += indent(1) + "refDes             = " + generalProperties.refDes             + newLine();
    str += indent(1) + "partValue          = " + generalProperties.partValue          + newLine();
    str += indent(1) + "implementationType = " + to_string(generalProperties.implementationType)    + newLine();
    str += indent(1) + "pinNameVisible     = " + std::to_string(generalProperties.pinNameVisible)   + newLine();
    str += indent(1) + "pinNameRotate      = " + std::to_string(generalProperties.pinNameRotate)    + newLine();
    str += indent(1) + "pinNumberVisible   = " + std::to_string(generalProperties.pinNumberVisible) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const GeneralProperties& generalProperties)
{
    os << to_string(generalProperties);

    return os;
}


#endif // GENERALPROPERTIES_H