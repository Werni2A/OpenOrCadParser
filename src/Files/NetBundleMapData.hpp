#ifndef NETBUNDLEMAPDATA_H
#define NETBUNDLEMAPDATA_H


#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../General.hpp"


struct NetBundleMapData
{

};


[[maybe_unused]]
static std::string to_string(const NetBundleMapData& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const NetBundleMapData& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // NETBUNDLEMAPDATA_H