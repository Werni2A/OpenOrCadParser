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
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::Properties, localFutureLst);

    readPreamble();

    ref = ds.readStringLenZeroTerm();

    spdlog::trace("ref = {}", ref);

    // @todo Probably a string
    ds.assumeData({0x00, 0x00, 0x00}, fmt::format("{}: 0", getMethodName(this, __func__)));

    localFutureLst.readRestOfStructure(); // @note Is equal to TrailingProperties::read()

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}