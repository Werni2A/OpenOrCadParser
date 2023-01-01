#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructT0x1f.hpp"


// @todo Probably specifies the 'Package Properties'
StructT0x1f Parser::readStructT0x1f()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    StructT0x1f obj;

    obj.name = mDs.readStringLenZeroTerm();

    std::string unknownStr0 = mDs.readStringLenZeroTerm(); // @todo figure out
    spdlog::debug("{}: unknownStr0 = {}", __func__, unknownStr0);

    obj.refDes = mDs.readStringLenZeroTerm();

    std::string unknownStr1 = mDs.readStringLenZeroTerm(); // @todo figure out
    spdlog::debug("{}: unknownStr1 = {}", __func__, unknownStr1);

    obj.pcbFootprint = mDs.readStringLenZeroTerm();

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}