#ifndef STRUCTPROPERTIES_HPP
#define STRUCTPROPERTIES_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>


struct StructProperties
{
    std::string ref;
};


[[maybe_unused]]
static std::string to_string(const StructProperties& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}ref  = {}\n", indent(1), aObj.ref);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructProperties& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTPROPERTIES_HPP