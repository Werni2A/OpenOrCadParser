#ifndef SYMBOLPINBUS_H
#define SYMBOLPINBUS_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

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
static std::string to_string(const SymbolPinBus& symbolPinBus)
{
    std::string str;

    str += "SymbolPinBus:" + newLine();
    str += indent(1) + "name     = " + symbolPinBus.name + newLine();
    str += indent(1) + "startX   = " + std::to_string(symbolPinBus.startX) + newLine();
    str += indent(1) + "startY   = " + std::to_string(symbolPinBus.startY) + newLine();
    str += indent(1) + "hotptX   = " + std::to_string(symbolPinBus.hotptX) + newLine();
    str += indent(1) + "hotptY   = " + std::to_string(symbolPinBus.hotptY) + newLine();

    str += indent(1) + "pinShape:" + newLine();
    str += indent(to_string(symbolPinBus.pinShape), 2);

    str += indent(1) + "portType = " + to_string(symbolPinBus.portType) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const SymbolPinBus& symbolPinBus)
{
    os << to_string(symbolPinBus);

    return os;
}


#endif // SYMBOLPINBUS_H