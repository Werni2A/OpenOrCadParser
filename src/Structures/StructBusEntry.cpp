#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructBusEntry.hpp"


void StructBusEntry::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::BusEntry, localFutureLst);

    readPreamble();

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

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}