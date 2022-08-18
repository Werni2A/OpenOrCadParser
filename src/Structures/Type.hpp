#ifndef TYPE_H
#define TYPE_H


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../Enums/ComponentType.hpp"


// @note This is used in `$Type$` files
struct Type
{
    std::string name;
    ComponentType componentType;
};


[[maybe_unused]]
static std::string to_string(const Type& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);
    str += fmt::format("{}componentType = {}\n", indent(1), to_string(aObj.componentType));

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Type& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // TYPE_H