#ifndef STREAMNETBUNDLEMAPDATA_HPP
#define STREAMNETBUNDLEMAPDATA_HPP


#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"


struct StreamNetBundleMapData
{

};


[[maybe_unused]]
static std::string to_string(const StreamNetBundleMapData& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamNetBundleMapData& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMNETBUNDLEMAPDATA_HPP