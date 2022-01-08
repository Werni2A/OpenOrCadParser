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
static std::string to_string(const GeneralProperties& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "implementationPath = " + aObj.implementationPath + newLine();
    str += indent(1) + "implementation     = " + aObj.implementation     + newLine();
    str += indent(1) + "refDes             = " + aObj.refDes             + newLine();
    str += indent(1) + "partValue          = " + aObj.partValue          + newLine();
    str += indent(1) + "implementationType = " + to_string(aObj.implementationType)    + newLine();
    str += indent(1) + "pinNameVisible     = " + std::to_string(aObj.pinNameVisible)   + newLine();
    str += indent(1) + "pinNameRotate      = " + std::to_string(aObj.pinNameRotate)    + newLine();
    str += indent(1) + "pinNumberVisible   = " + std::to_string(aObj.pinNumberVisible) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const GeneralProperties& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // GENERALPROPERTIES_H