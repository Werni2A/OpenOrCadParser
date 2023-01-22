#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructWire.hpp"


void StructWire::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    // @todo this 4 Byte and the following 4 byte ID
    //       might be swapped. I need to verify this!
    mDs.get().printUnknownData(4, fmt::format("{}: 0", __func__));

    id = mDs.get().readUint32();

    spdlog::debug("id = {}", id);

    color = ToColor(mDs.get().readUint32());

    spdlog::debug("color = {}", ::to_string(color));

    startX = mDs.get().readInt32();
    startY = mDs.get().readInt32();

    spdlog::debug("startX = {}", startX);
    spdlog::debug("startY = {}", startY);

    endX = mDs.get().readInt32();
    endY = mDs.get().readInt32();

    spdlog::debug("endX = {}", endX);
    spdlog::debug("endY = {}", endY);

    mDs.get().printUnknownData(1, fmt::format("{}: 1", __func__));

    const uint16_t lenAliases = mDs.get().readUint16();

    spdlog::debug("lenAliases = {}", lenAliases);

    for(size_t i = 0; i < lenAliases; ++i)
    {
        aliases.push_back(dynamic_pointer_cast<StructAlias>(readStructure()));
    }

    const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

    spdlog::debug("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    lineWidth = ToLineWidth(mDs.get().readUint32());

    spdlog::debug("lineWidth = {}", ::to_string(lineWidth));

    lineStyle = ToLineStyle(mDs.get().readUint32());

    spdlog::debug("lineStyle = {}", ::to_string(lineStyle));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}