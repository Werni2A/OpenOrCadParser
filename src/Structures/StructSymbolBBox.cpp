#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Structures/StructSymbolBBox.hpp"

// @todo is probably trailing data and not a struct object!
void StructSymbolBBox::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    x1 = ds.readInt16();
    y1 = ds.readInt16();
    x2 = ds.readInt16();
    y2 = ds.readInt16();

    // @todo not sure weather this belongs to the structure or should be outside of it
    ds.printUnknownData(4, fmt::format("{}: 0", getMethodName(this, __func__)));

    // localFutureLst.readRestOfStructure();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}