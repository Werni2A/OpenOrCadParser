#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamPackage.hpp"


void StreamPackage::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const uint16_t lenProperties = mDs.get().readUint16();

    spdlog::trace("lenProperties = {}", lenProperties);

    for(size_t i = 0u; i < lenProperties; ++i)
    {
        properties.push_back(dynamic_pointer_cast<StructProperties>(readStructure()));

        const uint16_t lenPrimitives = mDs.get().readUint16();

        spdlog::trace("lenPrimitives = {}", lenPrimitives);

        for(size_t i = 0u; i < lenPrimitives; ++i)
        {
            primitives.push_back(dynamic_pointer_cast<StructPrimitives>(readStructure()));
        }

        const uint16_t lenSymbolPins = mDs.get().readUint16();

        spdlog::trace("lenSymbolPins = {}", lenSymbolPins);

        for(size_t i = 0u; i < lenSymbolPins; ++i)
        {
            symbolPins.push_back(dynamic_pointer_cast<StructSymbolPin>(readStructure()));

            // @todo This hack should probably be moved into StructSymbolPin
            const uint8_t early_out = mDs.get().peek(1)[0];
            spdlog::debug("early_out = {}", early_out);

            if(early_out == 0U)
            {
                // @todo does not always occur, even in the same file. Maybe its some byte alignment?
                mDs.get().printUnknownData(1, "Early Out Indicator");
                break;
            }
        }

        const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

        spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

        for(size_t i = 0u; i < lenSymbolDisplayProps; ++i)
        {
            symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
        }

        const std::string someStr0 = mDs.get().readStringLenZeroTerm();
        const std::string someStr1 = mDs.get().readStringLenZeroTerm(); // @todo Maybe incorrect
        const std::string someStr2 = mDs.get().readStringLenZeroTerm();
        const std::string someStr3 = mDs.get().readStringLenZeroTerm();

        spdlog::trace("someStr0 = {}", someStr0);
        spdlog::trace("someStr1 = {}", someStr1);
        spdlog::trace("someStr2 = {}", someStr2);
        spdlog::trace("someStr3 = {}", someStr3);

        mDs.get().printUnknownData(2);

    }

    t0x1f = dynamic_pointer_cast<StructT0x1f>(readStructure());

    const uint16_t lenPinIdxMappings = mDs.get().readUint16();

    spdlog::trace("lenPinIdxMappings = {}", lenPinIdxMappings);

    for(size_t i = 0u; i < lenPinIdxMappings; ++i)
    {
        pinIdxMappings.push_back(dynamic_pointer_cast<StructPinIdxMapping>(readStructure()));
    }

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}