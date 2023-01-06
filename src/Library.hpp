#ifndef LIBRARY_HPP
#define LIBRARY_HPP


#include <cstdint>
#include <memory>
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


class Library
{
public:

    std::unique_ptr<StreamAdminData>        adminData;
    std::unique_ptr<StreamDsnStream>        dsnStream;
    std::unique_ptr<StreamNetBundleMapData> netBundleMapData;
    std::unique_ptr<StreamHSObjects>        hsObjects;

    std::unique_ptr<StreamDirectoryStruct>  cellsDir;
    std::unique_ptr<StreamDirectoryStruct>  exportBlocksDir;
    std::unique_ptr<StreamDirectoryStruct>  graphicsDir;
    std::unique_ptr<StreamDirectoryStruct>  packagesDir;
    std::unique_ptr<StreamDirectoryStruct>  partsDir;
    std::unique_ptr<StreamDirectoryStruct>  symbolsDir;
    std::unique_ptr<StreamDirectoryStruct>  viewsDir;

    std::unique_ptr<StreamLibrary>          library;

    std::unique_ptr<StreamType>             graphicsTypes;
    std::unique_ptr<StreamType>             symbolsTypes;

    std::vector<std::unique_ptr<StreamPackage>> packages;
    std::vector<std::unique_ptr<StreamSymbol>>  symbols;
    // std::vector<std::unique_ptr<StreamCell>> cells;
};


extern Library* gLibrary; //!< This stores the content of the parsed library file


[[maybe_unused]]
static std::string to_string(const Library& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}exportBlocksDir:\n", indent(1));
    str += fmt::format("{}exportBlocksDir:\n", indent(1));
    str += indent(aObj.exportBlocksDir->to_string(), 2);

    str += fmt::format("{}graphicsDir:\n", indent(1));
    str += indent(aObj.graphicsDir->to_string(), 2);

    str += fmt::format("{}packagesDir:\n", indent(1));
    str += indent(aObj.packagesDir->to_string(), 2);

    str += fmt::format("{}partsDir:\n", indent(1));
    str += indent(aObj.partsDir->to_string(), 2);

    str += fmt::format("{}symbolsDir:\n", indent(1));
    str += indent(aObj.symbolsDir->to_string(), 2);

    str += fmt::format("{}cellsDir:\n", indent(1));
    str += indent(aObj.cellsDir->to_string(), 2);

    str += fmt::format("{}viewsDir:\n", indent(1));
    str += indent(aObj.viewsDir->to_string(), 2);

    str += fmt::format("{}library:\n", indent(1));
    str += indent(aObj.library->to_string(), 2);

    str += fmt::format("{}graphicsTypes:\n", indent(1));
    for(size_t i = 0u; i < aObj.graphicsTypes->types.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.graphicsTypes->types[i].to_string()), 2);
    }

    str += fmt::format("{}symbolsTypes:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolsTypes->types.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.symbolsTypes->types[i].to_string()), 2);
    }

    str += fmt::format("{}packages:\n", indent(1));
    for(size_t i = 0u; i < aObj.packages.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.packages[i]->to_string()), 2);
    }

    str += fmt::format("{}symbols:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbols.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.symbols[i]->to_string()), 2);
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