#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructOffPageSymbol.hpp"


void StructOffPageSymbol::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::OffPageSymbol, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    StructSthInPages0::read_raw(aVersion, localFutureLst);

    const uint16_t lenSymbolPins = ds.readUint16();

    spdlog::trace("lenSymbolPins = {}", lenSymbolPins);

    for(size_t i = 0u; i < lenSymbolPins; ++i)
    {
        symbolPins.push_back(dynamic_pointer_cast<StructSymbolPin>(readStructure()));
    }

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0u; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}