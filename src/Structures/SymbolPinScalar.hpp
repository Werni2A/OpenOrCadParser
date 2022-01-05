#ifndef SYMBOLPINSCALAR_H
#define SYMBOLPINSCALAR_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

#include "../PinShape.hpp"
#include "../Enums/PortType.hpp"

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
static std::string to_string(const SymbolPinScalar& symbolPinScalar)
{
    std::string str;

    str += "SymbolPinScalar:" + newLine();
    str += indent(1) + "name     = " + symbolPinScalar.name + newLine();
    str += indent(1) + "startX   = " + std::to_string(symbolPinScalar.startX) + newLine();
    str += indent(1) + "startY   = " + std::to_string(symbolPinScalar.startY) + newLine();
    str += indent(1) + "hotptX   = " + std::to_string(symbolPinScalar.hotptX) + newLine();
    str += indent(1) + "hotptY   = " + std::to_string(symbolPinScalar.hotptY) + newLine();

    str += indent(1) + "pinShape:" + newLine();
    str += indent(to_string(symbolPinScalar.pinShape), 2);

    str += indent(1) + "portType = " + to_string(symbolPinScalar.portType) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const SymbolPinScalar& symbolPinScalar)
{
    os << to_string(symbolPinScalar);

    return os;
}


#endif // SYMBOLPINSCALAR_H