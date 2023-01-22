#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructProperties2.hpp"


void StructProperties2::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    name = mDs.get().readStringLenZeroTerm();

    mDs.get().assumeData({0x00, 0x00, 0x00}, fmt::format("{}: 0", __func__)); // Unknown but probably string

    refDes = mDs.get().readStringLenZeroTerm();

    mDs.get().assumeData({0x00, 0x00, 0x00}, fmt::format("{}: 1", __func__)); // Unknown but probably string

    footprint = mDs.get().readStringLenZeroTerm();

    sectionCount = mDs.get().readUint16(); // @todo has this something to do with units? Or was this just bad naming from myself?

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}