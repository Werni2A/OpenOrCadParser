#ifndef STRUCTT0X10_HPP
#define STRUCTT0X10_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"


struct StructT0x10
{

};


[[maybe_unused]]
static std::string to_string(const StructT0x10& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    // str += fmt::format("x1     = {}\n", aObj.x1);
    // str += fmt::format("y1     = {}\n", aObj.y1);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructT0x10& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTT0X10_HPP