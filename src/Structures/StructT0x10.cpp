#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructT0x10.hpp"


StructT0x10 Parser::readStructT0x10()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructT0x10 obj;

    spdlog::critical("{}: Not implemented!", __func__);

    if(thisFuture.has_value())
    {
        mDs.printUnknownData(thisFuture.value().getByteLen(), fmt::format("{}: 0", __func__));
    }
    else
    {
        mDs.printUnknownData(16, fmt::format("{}: 0", __func__));
    }

    sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}