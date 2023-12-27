#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "GenericParser.hpp"
#include "Streams/StreamPackage.hpp"


void StreamPackage::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const uint16_t lenProperties = ds.readUint16();

    spdlog::trace("lenProperties = {}", lenProperties);

    for(size_t i = 0u; i < lenProperties; ++i)
    {
        properties.push_back(dynamic_pointer_cast<StructProperties>(parser.readStructure()));

        const uint16_t lenPrimitives = ds.readUint16();

        spdlog::trace("lenPrimitives = {}", lenPrimitives);

        for(size_t i = 0u; i < lenPrimitives; ++i)
        {
            primitives.push_back(dynamic_pointer_cast<StructPrimitives>(parser.readStructure()));
        }
    }

    t0x1f = dynamic_pointer_cast<StructT0x1f>(parser.readStructure());

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}