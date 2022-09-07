#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructSymbolBBox.hpp"


// @todo is probably trailing data and not a struct object!
StructSymbolBBox Parser::readStructSymbolBBox()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructSymbolBBox obj;

    obj.x1 = mDs.readInt16();
    obj.y1 = mDs.readInt16();
    obj.x2 = mDs.readInt16();
    obj.y2 = mDs.readInt16();

    // @todo not sure weather this belongs to the structure or should be outside of it
    mDs.printUnknownData(4, fmt::format("{}: 0", __func__));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}