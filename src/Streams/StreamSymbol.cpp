#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamSymbol.hpp"


void StreamSymbol::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    structures.push_back(readStructure());

    mDs.get().printUnknownData(4, fmt::format("{}: 0", __func__));

    const uint16_t geometryCount = mDs.get().readUint16();
    spdlog::debug("geometryCount = {}", geometryCount);

    for(size_t i = 0u; i < geometryCount; ++i)
    {
        spdlog::debug("i of geometryCount = {}", i);

        if(i > 0u)
        {
            if(gFileFormatVersion == FileFormatVersion::B)
            {
                auto_read_prefixes();
            }

            if(gFileFormatVersion >= FileFormatVersion::B)
            {
                readPreamble();
            }
        }

        const Primitive primitive = readPrefixPrimitive();

        readPrimitive(primitive); // @todo add to obj
    }

    readPreamble();

    mDs.get().printUnknownData(8);

    // @todo Trailing data could be SymbolBBox
    // readSymbolBBox(); // @todo push structure

    const uint16_t len = mDs.get().readUint16();

    spdlog::info("len = {}", len);

    for(size_t i = 0u; i < len; ++i)
    {
        structures.push_back(readStructure());
    }

    const uint16_t len2 = mDs.get().readUint16();

    spdlog::info("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        structures.push_back(readStructure());
    }

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}