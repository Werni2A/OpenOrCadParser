#ifndef HSOBJECTS_H
#define HSOBJECTS_H


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../General.hpp"


struct HSObjects
{

};


[[maybe_unused]]
static std::string to_string(const HSObjects& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const HSObjects& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // HSOBJECTS_H