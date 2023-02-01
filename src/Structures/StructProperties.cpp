#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructProperties.hpp"


void StructProperties::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    FutureDataLst localFutureLst{mDs};

    auto_read_prefixes(Structure::Properties, localFutureLst);

    readPreamble();

    ref = mDs.get().readStringLenZeroTerm();

    spdlog::trace("ref = {}", ref);

    // @todo Probably a string
    mDs.get().assumeData({0x00, 0x00, 0x00}, fmt::format("{}: 0", getMethodName(this, __func__)));

    localFutureLst.readRestOfStructure(); // @note Is equal to TrailingProperties::read()

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}