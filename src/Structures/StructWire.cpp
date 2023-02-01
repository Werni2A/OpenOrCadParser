#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructWire.hpp"


void StructWire::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    FutureDataLst localFutureLst{mDs};

    auto_read_prefixes({Structure::WireBus, Structure::WireScalar}, localFutureLst);

    readPreamble();

    // @todo this 4 Byte and the following 4 byte ID
    //       might be swapped. I need to verify this!
    mDs.get().printUnknownData(4, fmt::format("{}: 0", getMethodName(this, __func__)));

    id = mDs.get().readUint32();

    spdlog::trace("id = {}", id);

    color = ToColor(mDs.get().readUint32());

    spdlog::trace("color = {}", ::to_string(color));

    startX = mDs.get().readInt32();
    startY = mDs.get().readInt32();

    spdlog::trace("startX = {}", startX);
    spdlog::trace("startY = {}", startY);

    endX = mDs.get().readInt32();
    endY = mDs.get().readInt32();

    spdlog::trace("endX = {}", endX);
    spdlog::trace("endY = {}", endY);

    mDs.get().printUnknownData(1, fmt::format("{}: 1", getMethodName(this, __func__)));

    const uint16_t lenAliases = mDs.get().readUint16();

    spdlog::trace("lenAliases = {}", lenAliases);

    for(size_t i = 0; i < lenAliases; ++i)
    {
        aliases.push_back(dynamic_pointer_cast<StructAlias>(readStructure()));
    }

    const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    lineWidth = ToLineWidth(mDs.get().readUint32());

    spdlog::trace("lineWidth = {}", ::to_string(lineWidth));

    lineStyle = ToLineStyle(mDs.get().readUint32());

    spdlog::trace("lineStyle = {}", ::to_string(lineStyle));

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}