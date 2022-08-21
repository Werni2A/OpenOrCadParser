#ifndef PACKAGE_H
#define PACKAGE_H


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../Structures/GeneralProperties.hpp"
#include "../Structures/GeometrySpecification.hpp"
#include "../Structures/SymbolPinScalar.hpp"
#include "../Structures/PinIdxMapping.hpp"
#include "../Structures/Properties.hpp"
#include "../Structures/T0x1f.hpp"
#include "../Structures/SymbolDisplayProp.hpp"


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
static std::string to_string(const Package& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}generalProperties\n", indent(1));
    str += indent(to_string(aObj.generalProperties), 2);

    str += fmt::format("{}properties:\n", indent(1));
    for(size_t i = 0u; i < aObj.properties.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.properties[i])), 2);
    }

    str += fmt::format("{}geometrySpecifications:\n", indent(1));
    for(size_t i = 0u; i < aObj.geometrySpecifications.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.geometrySpecifications[i])), 2);
    }

    str += fmt::format("{}symbolPinScalars:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolPinScalars.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.symbolPinScalars[i])), 2);
    }

    str += fmt::format("{}t0x1fs:\n", indent(1));
    for(size_t i = 0u; i < aObj.t0x1fs.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.t0x1fs[i])), 2);
    }

    str += fmt::format("{}pinIdxMappings:\n", indent(1));
    for(size_t i = 0u; i < aObj.pinIdxMappings.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.pinIdxMappings[i])), 2);
    }

    str += fmt::format("{}globalSymbols:\n", indent(1));
    for(size_t i = 0u; i < aObj.globalSymbols.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.globalSymbols[i])), 2);
    }

    str += fmt::format("{}portSymbols:\n", indent(1));
    for(size_t i = 0u; i < aObj.portSymbols.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.portSymbols[i])), 2);
    }

    str += fmt::format("{}offPageSymbols:\n", indent(1));
    for(size_t i = 0u; i < aObj.offPageSymbols.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.offPageSymbols[i])), 2);
    }

    str += fmt::format("{}symbolDisplayProps:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolDisplayProps.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.symbolDisplayProps[i])), 2);
    }

    str += fmt::format("{}symbolVectors:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolVectors.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.symbolVectors[i])), 2);
    }

    str += fmt::format("{}titleBlockSymbols:\n", indent(1));
    for(size_t i = 0u; i < aObj.titleBlockSymbols.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.titleBlockSymbols[i])), 2);
    }

    str += fmt::format("{}ercSymbols:\n", indent(1));
    for(size_t i = 0u; i < aObj.ercSymbols.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.ercSymbols[i])), 2);
    }

    str += fmt::format("{}pinShapeSymbols:\n", indent(1));
    for(size_t i = 0u; i < aObj.pinShapeSymbols.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.pinShapeSymbols[i])), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Package& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PACKAGE_H