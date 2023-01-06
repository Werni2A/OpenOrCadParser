#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructProperties.hpp"
#include "Structures/TrailingProperties.hpp"


void StructProperties::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    ref = mDs.get().readStringLenZeroTerm();

    // @todo Probably a string
    mDs.get().assumeData({0x00, 0x00, 0x00}, fmt::format("{}: 0", __func__));

    sanitizeThisFutureSize(thisFuture);

    if(checkTrailingFuture().has_value())
    {
        TrailingProperties trailing{mDs};
        trailing.read();
    }

    spdlog::debug(getClosingMsg(__func__, mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}