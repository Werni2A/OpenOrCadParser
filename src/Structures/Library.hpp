#ifndef LIBRARY_H
#define LIBRARY_H


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "DirectoryStruct.hpp"
#include "Files/AdminData.hpp"
#include "Files/DsnStream.hpp"
#include "Files/NetBundleMapData.hpp"
#include "Files/HSObjects.hpp"
#include "Package.hpp"
#include "SymbolsLibrary.hpp"
#include "Type.hpp"


struct Library
{
    AdminData        adminData;
    DsnStream        dsnStream;
    NetBundleMapData netBundleMapData;
    HSObjects        hsObjects;

    DirectoryStruct cellsDir;
    DirectoryStruct exportBlocksDir;
    DirectoryStruct graphicsDir;
    DirectoryStruct packagesDir;
    DirectoryStruct partsDir;
    DirectoryStruct symbolsDir;
    DirectoryStruct viewsDir;

    SymbolsLibrary symbolsLibrary; // @todo rename just to 'library'?

    std::vector<Type> graphicsTypes;
    std::vector<Type> symbolsTypes;

    std::vector<Package> packages;
    // std::vector<Symbol> symbols;
    // std::vector<Cell> cells;
};


[[maybe_unused]]
static std::string to_string(const Library& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}exportBlocksDir:\n", indent(1));
    str += fmt::format("{}exportBlocksDir:\n", indent(1));
    str += indent(to_string(aObj.exportBlocksDir), 2);

    str += fmt::format("{}graphicsDir:\n", indent(1));
    str += indent(to_string(aObj.graphicsDir), 2);

    str += fmt::format("{}packagesDir:\n", indent(1));
    str += indent(to_string(aObj.packagesDir), 2);

    str += fmt::format("{}partsDir:\n", indent(1));
    str += indent(to_string(aObj.partsDir), 2);

    str += fmt::format("{}symbolsDir:\n", indent(1));
    str += indent(to_string(aObj.symbolsDir), 2);

    str += fmt::format("{}cellsDir:\n", indent(1));
    str += indent(to_string(aObj.cellsDir), 2);

    str += fmt::format("{}viewsDir:\n", indent(1));
    str += indent(to_string(aObj.viewsDir), 2);

    str += fmt::format("{}symbolsLibrary:\n", indent(1));
    str += indent(to_string(aObj.symbolsLibrary), 2);

    str += fmt::format("{}symbolsTypes:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolsTypes.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.symbolsTypes[i])), 2);
    }

    str += fmt::format("{}packages:\n", indent(1));
    for(size_t i = 0u; i < aObj.packages.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.packages[i])), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Library& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // LIBRARY_H