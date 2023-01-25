#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructProperties.hpp"


void StructProperties::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    ref = mDs.get().readStringLenZeroTerm();

    // @todo Probably a string
    mDs.get().assumeData({0x00, 0x00, 0x00}, fmt::format("{}: 0", __func__));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture(); // @note Is equal to TrailingProperties::read()

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}