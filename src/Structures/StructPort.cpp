#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructPort.hpp"


void StructPort::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    mDs.get().printUnknownData(8, fmt::format("{}: 0", __func__));

    name = mDs.get().readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    mDs.get().printUnknownData(20, fmt::format("{}: 1", __func__));

    const uint16_t len = mDs.get().readUint16();

    // @todo Should be display property
    spdlog::trace("len = {}", len);

    for(size_t i = 0; i < len; ++i)
    {
        readStructure();
    }

    mDs.get().printUnknownData(10, fmt::format("{}: 2", __func__));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}