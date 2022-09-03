#ifndef STREAMADMINDATA_HPP
#define STREAMADMINDATA_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"


struct StreamAdminData
{

};


[[maybe_unused]]
static std::string to_string(const StreamAdminData& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamAdminData& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMADMINDATA_HPP