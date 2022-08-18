#ifndef PROPERTIES_H
#define PROPERTIES_H


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>


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

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);
    str += fmt::format("{}ref  = {}\n", indent(1), aObj.ref);
    str += fmt::format("{}convertName = {}\n", indent(1), aObj.convertName);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Properties& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PROPERTIES_H