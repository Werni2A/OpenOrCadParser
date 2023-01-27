#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructT0x10.hpp"


void StructT0x10::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes(Structure::T0x10);

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    const uint16_t sth = mDs.get().readUint16();

    spdlog::trace("sth = {}", sth);

    const int16_t pointX = mDs.get().readInt16();
    const int16_t pointY = mDs.get().readInt16();

    spdlog::trace("pointX = {}", pointX);
    spdlog::trace("pointY = {}", pointY);

    const uint32_t maybeId = mDs.get().readUint32();

    spdlog::trace("maybeId = {}", maybeId);

    const uint32_t unkownInt = mDs.get().readUint32();

    spdlog::trace("unkownInt = {}", unkownInt);

    const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    sanitizeThisFutureSize(thisFuture);

    // readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}