#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructTitleBlockSymbol.hpp"


void StructTitleBlockSymbol::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::TitleBlockSymbol, localFutureLst);

    readPreamble();

    const std::string name = ds.readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    const std::string someStr = ds.readStringLenZeroTerm();

    spdlog::trace("someStr = {}", someStr);

    ds.printUnknownData(4, fmt::format("{}: 0", getMethodName(this, __func__)));

    const uint16_t len0 = ds.readUint16();

    for(size_t i = 0; i < len0; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();
        readPrimitive(primitive);
    }

    ds.printUnknownData(8, fmt::format("{}: 1", getMethodName(this, __func__)));

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

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}