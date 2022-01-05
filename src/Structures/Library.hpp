#ifndef LIBRARY_H
#define LIBRARY_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "Package.hpp"
#include "DirectoryStruct.hpp"
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
static std::string to_string(const Library& library)
{
    std::string str;

    str += "Library:" + newLine();

    str += indent(1) + "exportBlocksDir:" + newLine();
    str += indent(to_string(library.exportBlocksDir), 2);

    str += indent(1) + "graphicsDir:" + newLine();
    str += indent(to_string(library.graphicsDir), 2);

    str += indent(1) + "packagesDir:" + newLine();
    str += indent(to_string(library.packagesDir), 2);

    str += indent(1) + "partsDir:" + newLine();
    str += indent(to_string(library.partsDir), 2);

    str += indent(1) + "symbolsDir:" + newLine();
    str += indent(to_string(library.symbolsDir), 2);

    str += indent(1) + "cellsDir:" + newLine();
    str += indent(to_string(library.cellsDir), 2);

    str += indent(1) + "viewsDir:" + newLine();
    str += indent(to_string(library.viewsDir), 2);

    str += indent(1) + "symbolsLibrary:" + newLine();
    str += indent(to_string(library.symbolsLibrary), 2);

    str += indent(1) + "symbolsTypes:" + newLine();
    for(size_t i = 0u; i < library.symbolsTypes.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(library.symbolsTypes[i]), 2);
    }

    str += indent(1) + "packages:" + newLine();
    for(size_t i = 0u; i < library.packages.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(library.packages[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Library& library)
{
    os << to_string(library);

    return os;
}


#endif // LIBRARY_H