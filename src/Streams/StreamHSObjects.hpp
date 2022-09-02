#ifndef STREAMHSOBJECTS_HPP
#define STREAMHSOBJECTS_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"


struct StreamHSObjects
{

};


[[maybe_unused]]
static std::string to_string(const StreamHSObjects& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamHSObjects& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMHSOBJECTS_HPP