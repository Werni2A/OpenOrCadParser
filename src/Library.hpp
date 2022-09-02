#ifndef LIBRARY_HPP
#define LIBRARY_HPP


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Streams/StreamAdminData.hpp"
#include "Streams/StreamDirectoryStruct.hpp"
#include "Streams/StreamDsnStream.hpp"
#include "Streams/StreamHSObjects.hpp"
#include "Streams/StreamLibrary.hpp"
#include "Streams/StreamNetBundleMapData.hpp"
#include "Streams/StreamPackage.hpp"
#include "Streams/StreamSymbol.hpp"
#include "Streams/StreamType.hpp"


struct Library
{
    StreamAdminData        adminData;
    StreamDsnStream        dsnStream;
    StreamNetBundleMapData netBundleMapData;
    StreamHSObjects        hsObjects;

    StreamDirectoryStruct  cellsDir;
    StreamDirectoryStruct  exportBlocksDir;
    StreamDirectoryStruct  graphicsDir;
    StreamDirectoryStruct  packagesDir;
    StreamDirectoryStruct  partsDir;
    StreamDirectoryStruct  symbolsDir;
    StreamDirectoryStruct  viewsDir;

    StreamLibrary          library;

    std::vector<Type>      graphicsTypes;
    std::vector<Type>      symbolsTypes;

    std::vector<StreamPackage> packages;
    std::vector<StreamSymbol>  symbols;
    // std::vector<StreamCell> cells;
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

    str += fmt::format("{}library:\n", indent(1));
    str += indent(to_string(aObj.library), 2);

    str += fmt::format("{}graphicsTypes:\n", indent(1));
    for(size_t i = 0u; i < aObj.graphicsTypes.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.graphicsTypes[i])), 2);
    }

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

    str += fmt::format("{}symbols:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbols.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.symbols[i])), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Library& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // LIBRARY_HPP