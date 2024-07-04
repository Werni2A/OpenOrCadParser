#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructBusEntry.hpp"

void OOCP::StructBusEntry::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::BusEntry, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

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

    ds.printUnknownData(8, getMethodName(this, __func__) + ": 0");

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}