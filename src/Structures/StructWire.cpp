#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructWire.hpp"


void StructWire::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes({Structure::WireBus, Structure::WireScalar}, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    // @todo this 4 Byte and the following 4 byte ID
    //       might be swapped. I need to verify this!
    ds.printUnknownData(4, fmt::format("{}: 0", getMethodName(this, __func__)));

    id = ds.readUint32();

    spdlog::trace("id = {}", id);

    color = ToColor(ds.readUint32());

    spdlog::trace("color = {}", ::to_string(color));

    startX = ds.readInt32();
    startY = ds.readInt32();

    spdlog::trace("startX = {}", startX);
    spdlog::trace("startY = {}", startY);

    endX = ds.readInt32();
    endY = ds.readInt32();

    spdlog::trace("endX = {}", endX);
    spdlog::trace("endY = {}", endY);

    ds.printUnknownData(1, fmt::format("{}: 1", getMethodName(this, __func__)));

    const uint16_t lenAliases = ds.readUint16();

    spdlog::trace("lenAliases = {}", lenAliases);

    for(size_t i = 0; i < lenAliases; ++i)
    {
        aliases.push_back(dynamic_pointer_cast<StructAlias>(parser.readStructure()));
    }

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(parser.readStructure()));
    }

    lineWidth = ToLineWidth(ds.readUint32());

    spdlog::trace("lineWidth = {}", ::to_string(lineWidth));

    lineStyle = ToLineStyle(ds.readUint32());

    spdlog::trace("lineStyle = {}", ::to_string(lineStyle));

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}