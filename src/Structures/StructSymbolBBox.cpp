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
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    x1 = mDs.get().readInt16();
    y1 = mDs.get().readInt16();
    x2 = mDs.get().readInt16();
    y2 = mDs.get().readInt16();

    // @todo not sure weather this belongs to the structure or should be outside of it
    mDs.get().printUnknownData(4, fmt::format("{}: 0", __func__));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}