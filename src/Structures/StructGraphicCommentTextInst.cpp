#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructGraphicCommentTextInst.hpp"


StructGraphicCommentTextInst Parser::readStructGraphicCommentTextInst()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    StructGraphicCommentTextInst obj;

    mDs.printUnknownData(34, std::string(__func__) + " - 0");

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}