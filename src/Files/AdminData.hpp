#ifndef ADMINDATA_H
#define ADMINDATA_H


#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../General.hpp"


struct AdminData
{

};


[[maybe_unused]]
static std::string to_string(const AdminData& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const AdminData& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // ADMINDATA_H