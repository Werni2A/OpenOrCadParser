#ifndef SYMBOLPINBUS_H
#define SYMBOLPINBUS_H


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../PinShape.hpp"
#include "../Enums/PortType.hpp"


// @todo this looks exactly the same as SymbolPinScalar. Why is that?

struct SymbolPinBus
{
    std::string name;
    int32_t startX;
    int32_t startY;
    int32_t hotptX;
    int32_t hotptY;

    PinShape pinShape;

    PortType portType;
};


[[maybe_unused]]
static std::string to_string(const SymbolPinBus& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name     = {}\n", indent(1), aObj.name);
    str += fmt::format("{}startX   = {}\n", indent(1), aObj.startX);
    str += fmt::format("{}startY   = {}\n", indent(1), aObj.startY);
    str += fmt::format("{}hotptX   = {}\n", indent(1), aObj.hotptX);
    str += fmt::format("{}hotptY   = {}\n", indent(1), aObj.hotptY);

    str += fmt::format("{}pinShape:\n", indent(1));
    str += indent(to_string(aObj.pinShape), 2);

    str += fmt::format("{}portType = {}\n", indent(1), to_string(aObj.portType));

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const SymbolPinBus& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // SYMBOLPINBUS_H