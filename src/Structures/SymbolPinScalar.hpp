#ifndef SYMBOLPINSCALAR_H
#define SYMBOLPINSCALAR_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

#include "../Enums/PortType.hpp"
#include "../PinShape.hpp"


struct SymbolPinScalar
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
static std::string to_string(const SymbolPinScalar& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "name     = " + aObj.name + newLine();
    str += indent(1) + "startX   = " + std::to_string(aObj.startX) + newLine();
    str += indent(1) + "startY   = " + std::to_string(aObj.startY) + newLine();
    str += indent(1) + "hotptX   = " + std::to_string(aObj.hotptX) + newLine();
    str += indent(1) + "hotptY   = " + std::to_string(aObj.hotptY) + newLine();

    str += indent(1) + "pinShape:" + newLine();
    str += indent(to_string(aObj.pinShape), 2);

    str += indent(1) + "portType = " + to_string(aObj.portType) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const SymbolPinScalar& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // SYMBOLPINSCALAR_H