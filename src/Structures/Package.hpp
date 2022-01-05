#ifndef PACKAGE_H
#define PACKAGE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "GeneralProperties.hpp"
#include "GeometrySpecification.hpp"
#include "SymbolPinScalar.hpp"
#include "PinIdxMapping.hpp"
#include "Properties.hpp"
#include "T0x1f.hpp"
#include "SymbolDisplayProp.hpp"


// @todo split up into 'Package' and 'Symbol'
struct Package
{
    GeneralProperties generalProperties;

    std::vector<Properties>            properties;
    std::vector<GeometrySpecification> geometrySpecifications;
    std::vector<SymbolPinScalar>       symbolPinScalars;
    std::vector<T0x1f>                 t0x1fs;
    std::vector<PinIdxMapping>         pinIdxMappings;
    // @todo the following requires quite some restructuring as
    //       GeometrySpecification is only part of e.g. portSymbols
    //       therefore a class PortSymbol would be good. Same
    //       for all other symbol types.
    std::vector<GeometrySpecification> globalSymbols;
    std::vector<GeometrySpecification> portSymbols;
    std::vector<GeometrySpecification> offPageSymbols;
    std::vector<SymbolDisplayProp>     symbolDisplayProps;
    std::vector<GeometrySpecification> symbolVectors;
    std::vector<GeometrySpecification> titleBlockSymbols;
    std::vector<GeometrySpecification> ercSymbols;
    std::vector<GeometrySpecification> pinShapeSymbols;
};


[[maybe_unused]]
static std::string to_string(const Package& package)
{
    std::string str;

    str += "Package:" + newLine();

    str += indent(1) + "generalProperties" + newLine();
    str += indent(to_string(package.generalProperties), 2);

    str += indent(1) + "properties:" + newLine();
    for(size_t i = 0u; i < package.properties.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.properties[i]), 2);
    }

    str += indent(1) + "geometrySpecifications:" + newLine();
    for(size_t i = 0u; i < package.geometrySpecifications.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.geometrySpecifications[i]), 2);
    }

    str += indent(1) + "symbolPinScalars:" + newLine();
    for(size_t i = 0u; i < package.symbolPinScalars.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.symbolPinScalars[i]), 2);
    }

    str += indent(1) + "t0x1fs:" + newLine();
    for(size_t i = 0u; i < package.t0x1fs.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.t0x1fs[i]), 2);
    }

    str += indent(1) + "pinIdxMappings:" + newLine();
    for(size_t i = 0u; i < package.pinIdxMappings.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.pinIdxMappings[i]), 2);
    }

    str += indent(1) + "globalSymbols:" + newLine();
    for(size_t i = 0u; i < package.globalSymbols.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.globalSymbols[i]), 2);
    }

    str += indent(1) + "portSymbols:" + newLine();
    for(size_t i = 0u; i < package.portSymbols.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.portSymbols[i]), 2);
    }

    str += indent(1) + "offPageSymbols:" + newLine();
    for(size_t i = 0u; i < package.offPageSymbols.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.offPageSymbols[i]), 2);
    }

    str += indent(1) + "symbolDisplayProps:" + newLine();
    for(size_t i = 0u; i < package.symbolDisplayProps.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.symbolDisplayProps[i]), 2);
    }

    str += indent(1) + "symbolVectors:" + newLine();
    for(size_t i = 0u; i < package.symbolVectors.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.symbolVectors[i]), 2);
    }

    str += indent(1) + "titleBlockSymbols:" + newLine();
    for(size_t i = 0u; i < package.titleBlockSymbols.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.titleBlockSymbols[i]), 2);
    }

    str += indent(1) + "ercSymbols:" + newLine();
    for(size_t i = 0u; i < package.ercSymbols.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.ercSymbols[i]), 2);
    }

    str += indent(1) + "pinShapeSymbols:" + newLine();
    for(size_t i = 0u; i < package.pinShapeSymbols.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(package.pinShapeSymbols[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Package& package)
{
    os << to_string(package);

    return os;
}


#endif // PACKAGE_H