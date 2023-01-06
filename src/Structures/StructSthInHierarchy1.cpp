#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSthInHierarchy1.hpp"


void StructSthInHierarchy1::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    mDs.get().printUnknownData(27, fmt::format("{}: 0", __func__));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}