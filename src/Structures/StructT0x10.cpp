#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructT0x10.hpp"


void StructT0x10::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    spdlog::critical("{}: Not implemented!", __func__);

    if(thisFuture.has_value())
    {
        mDs.get().printUnknownData(thisFuture.value().getByteLen(), fmt::format("{}: 0", __func__));
    }
    else
    {
        mDs.get().printUnknownData(16, fmt::format("{}: 0", __func__));
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}