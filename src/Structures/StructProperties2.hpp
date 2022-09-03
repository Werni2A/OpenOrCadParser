#ifndef STRUCTPROPERTIES2_HPP
#define STRUCTPROPERTIES2_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>


struct StructProperties2
{
    std::string name;
    std::string refDes;
    std::string footprint;

    uint16_t sectionCount;
};


[[maybe_unused]]
static std::string to_string(const StructProperties2& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name   = {}\n", indent(1), aObj.name);
    str += fmt::format("{}refDes = {}\n", indent(1), aObj.refDes);
    str += fmt::format("{}footprint    = {}\n", indent(1), aObj.footprint);
    str += fmt::format("{}sectionCount = {}\n", indent(1), aObj.sectionCount);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructProperties2& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTPROPERTIES2_HPP