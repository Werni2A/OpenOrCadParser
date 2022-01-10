#ifndef TYPE_H
#define TYPE_H


#include <cstdint>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>

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

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "name = " + aObj.name + newLine();
    str += indent(1) + "componentType = " + to_string(aObj.componentType) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Type& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // TYPE_H