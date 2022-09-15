#ifndef STRUCTALIAS_HPP
#define STRUCTALIAS_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/Color.hpp"
#include "Enums/Rotation.hpp"
#include "General.hpp"


struct StructAlias
{
    int32_t  locX;
    int32_t  locY;

    Color    color;
    Rotation rotation;

    std::string name;
};


[[maybe_unused]]
static std::string to_string(const StructAlias& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("locX     = {}\n", aObj.locX);
    str += fmt::format("locY     = {}\n", aObj.locY);
    str += fmt::format("color    = {}\n", to_string(aObj.color));
    str += fmt::format("rotation = {}\n", to_string(aObj.rotation));
    str += fmt::format("name     = {}\n", aObj.name);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructAlias& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTALIAS_HPP