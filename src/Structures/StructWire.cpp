#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructWire.hpp"

void OOCP::StructWire::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes({Structure::WireBus, Structure::WireScalar}, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    // @todo this 4 Byte and the following 4 byte ID
    //       might be swapped. I need to verify this!
    ds.printUnknownData(4, fmt::format("{}: 0", getMethodName(this, __func__)));

    id = ds.readUint32();

    mCtx.mLogger.trace("id = {}", id);

    color = ToColor(ds.readUint32());

    mCtx.mLogger.trace("color = {}", OOCP::to_string(color));

    startX = ds.readInt32();
    startY = ds.readInt32();

    mCtx.mLogger.trace("startX = {}", startX);
    mCtx.mLogger.trace("startY = {}", startY);

    endX = ds.readInt32();
    endY = ds.readInt32();

    mCtx.mLogger.trace("endX = {}", endX);
    mCtx.mLogger.trace("endY = {}", endY);

    ds.printUnknownData(1, fmt::format("{}: 1", getMethodName(this, __func__)));

    const uint16_t lenAliases = ds.readUint16();

    mCtx.mLogger.trace("lenAliases = {}", lenAliases);

    for(size_t i = 0; i < lenAliases; ++i)
    {
        aliases.push_back(dynamic_pointer_cast<StructAlias>(parser.readStructure()));
    }

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    mCtx.mLogger.trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(parser.readStructure()));
    }

    lineWidth = ToLineWidth(ds.readUint32());

    mCtx.mLogger.trace("lineWidth = {}", OOCP::to_string(lineWidth));

    lineStyle = ToLineStyle(ds.readUint32());

    mCtx.mLogger.trace("lineStyle = {}", OOCP::to_string(lineStyle));

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}