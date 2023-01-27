#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructT0x1f.hpp"


// @todo Probably specifies the 'Package Properties'
void StructT0x1f::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes(Structure::T0x1f);

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    name = mDs.get().readStringLenZeroTerm();

    std::string unknownStr0 = mDs.get().readStringLenZeroTerm(); // @todo figure out
    spdlog::trace("{}: unknownStr0 = {}", __func__, unknownStr0);

    refDes = mDs.get().readStringLenZeroTerm();

    std::string unknownStr1 = mDs.get().readStringLenZeroTerm(); // @todo figure out
    spdlog::trace("{}: unknownStr1 = {}", __func__, unknownStr1);

    pcbFootprint = mDs.get().readStringLenZeroTerm();

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}