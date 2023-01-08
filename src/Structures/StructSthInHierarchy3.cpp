#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSthInHierarchy3.hpp"


void StructSthInHierarchy3::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    mDs.get().printUnknownData(12, fmt::format("{}: 2", __func__));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}