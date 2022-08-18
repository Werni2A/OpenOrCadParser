#ifndef GENERALPROPERTIES_H
#define GENERALPROPERTIES_H


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

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

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}implementationPath = {}\n", indent(1), aObj.implementationPath);
    str += fmt::format("{}implementation     = {}\n", indent(1), aObj.implementation);
    str += fmt::format("{}refDes             = {}\n", indent(1), aObj.refDes);
    str += fmt::format("{}partValue          = {}\n", indent(1), aObj.partValue);
    str += fmt::format("{}implementationType = {}\n", indent(1), to_string(aObj.implementationType));
    str += fmt::format("{}pinNameVisible     = {}\n", indent(1), aObj.pinNameVisible);
    str += fmt::format("{}pinNameRotate      = {}\n", indent(1), aObj.pinNameRotate);
    str += fmt::format("{}pinNumberVisible   = {}\n", indent(1), aObj.pinNumberVisible);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const GeneralProperties& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // GENERALPROPERTIES_H