#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamSymbol.hpp"


void StreamSymbol::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    spdlog::critical("VERIFYING StreamSymbol Structure10 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure

    mDs.get().printUnknownData(4, fmt::format("{}: 0", __func__));

    const uint16_t geometryCount = mDs.get().readUint16();

    spdlog::trace("geometryCount = {}", geometryCount);

    for(size_t i = 0u; i < geometryCount; ++i)
    {
        spdlog::trace("i of geometryCount = {}", i);

        // @todo This whole code sequence does not make much sense
        //       Try if readStructure() works out of the box
        if(i > 0u)
        {
            // @todo Check if the `if` statement is really necessary
            //       auto_read_prefixes should be able to handle
            //       a value of 0
            if(gFileFormatVersion == FileFormatVersion::B)
            {
                spdlog::critical("VERIFYING StreamSymbol Structure11 is {}", NAMEOF_TYPE_RTTI(auto_read_prefixes()));
            }

            // @todo check if if works without the `if` statement
            //       readPreamble should be able to skip it if it's
            //       not present
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

    const uint16_t lenSymbolPins = mDs.get().readUint16();

    spdlog::trace("lenSymbolPins = {}", lenSymbolPins);

    for(size_t i = 0u; i < lenSymbolPins; ++i)
    {
        symbolPins.push_back(dynamic_pointer_cast<StructSymbolPin>(readStructure()));
    }

    const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0u; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}