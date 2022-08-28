#ifndef PROPERTIESTRAILING_H
#define PROPERTIESTRAILING_H


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>


struct PropertiesTrailing
{
    std::string name;
    std::string convertName;
};


[[maybe_unused]]
static std::string to_string(const PropertiesTrailing& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);
    str += fmt::format("{}convertName = {}\n", indent(1), aObj.convertName);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PropertiesTrailing& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PROPERTIESTRAILING_H