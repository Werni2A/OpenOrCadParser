#ifndef LIBRARY_H
#define LIBRARY_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "DirectoryStruct.hpp"
#include "Package.hpp"
#include "SymbolsLibrary.hpp"


struct Library
{
    DirectoryStruct exportBlocksDir;
    DirectoryStruct graphicsDir;
    DirectoryStruct packagesDir;
    DirectoryStruct partsDir;
    DirectoryStruct symbolsDir;
    DirectoryStruct cellsDir;
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

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();

    str += indent(1) + "exportBlocksDir:" + newLine();
    str += indent(to_string(aObj.exportBlocksDir), 2);

    str += indent(1) + "graphicsDir:" + newLine();
    str += indent(to_string(aObj.graphicsDir), 2);

    str += indent(1) + "packagesDir:" + newLine();
    str += indent(to_string(aObj.packagesDir), 2);

    str += indent(1) + "partsDir:" + newLine();
    str += indent(to_string(aObj.partsDir), 2);

    str += indent(1) + "symbolsDir:" + newLine();
    str += indent(to_string(aObj.symbolsDir), 2);

    str += indent(1) + "cellsDir:" + newLine();
    str += indent(to_string(aObj.cellsDir), 2);

    str += indent(1) + "viewsDir:" + newLine();
    str += indent(to_string(aObj.viewsDir), 2);

    str += indent(1) + "symbolsLibrary:" + newLine();
    str += indent(to_string(aObj.symbolsLibrary), 2);

    str += indent(1) + "symbolsTypes:" + newLine();
    for(size_t i = 0u; i < aObj.symbolsTypes.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.symbolsTypes[i]), 2);
    }

    str += indent(1) + "packages:" + newLine();
    for(size_t i = 0u; i < aObj.packages.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.packages[i]), 2);
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