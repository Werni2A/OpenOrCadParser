#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructBusEntry.hpp"


void StructBusEntry::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

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

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}