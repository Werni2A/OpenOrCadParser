#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructGraphicCommentTextInst.hpp"


void StructGraphicCommentTextInst::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    mDs.get().printUnknownData(34, std::string(__func__) + " - 0");

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}