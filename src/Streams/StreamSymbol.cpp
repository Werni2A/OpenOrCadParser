#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Parser.hpp"
#include "Streams/StreamSymbol.hpp"


StreamSymbol Parser::readStreamSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    StreamSymbol obj;

    Structure structure = auto_read_prefixes();
    obj.structures.push_back(readStructure(structure));

    mDs.printUnknownData(4, fmt::format("{}: 0", __func__));

    const uint16_t geometryCount = mDs.readUint16();
    spdlog::debug("geometryCount = {}", geometryCount);

    for(size_t i = 0u; i < geometryCount; ++i)
    {
        spdlog::debug("i of geometryCount = {}", i);

        if(i > 0u)
        {
            if(mFileFormatVersion == FileFormatVersion::B)
            {
                Structure structure = auto_read_prefixes();
            }

            if(mFileFormatVersion >= FileFormatVersion::B)
            {
                readPreamble();
            }
        }

        const Primitive primitive = readPrefixPrimitive();

        readPrimitive(primitive); // @todo add to obj
    }

    readPreamble();

    mDs.printUnknownData(8);

    // @todo Trailing data could be SymbolBBox
    // readSymbolBBox(); // @todo push structure

    const uint16_t len = mDs.readUint16();

    spdlog::info("len = {}", len);

    for(size_t i = 0u; i < len; ++i)
    {
        Structure structure = auto_read_prefixes();
        obj.structures.push_back(readStructure(structure));
    }

    const uint16_t len2 = mDs.readUint16();

    spdlog::info("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        Structure structure = auto_read_prefixes();
        obj.structures.push_back(readStructure(structure));
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}