#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructBusEntry.hpp"


void StructBusEntry::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::BusEntry, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

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

    ds.printUnknownData(8, getMethodName(this, __func__) + ": 0");

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}