#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructTitleBlockSymbol.hpp"


void StructTitleBlockSymbol::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    auto_read_prefixes();

    readPreamble();

    const std::string str0 = mDs.get().readStringLenZeroTerm();

    spdlog::trace("str0 = {}", str0);

    const std::string str1 = mDs.get().readStringLenZeroTerm();

    spdlog::trace("str1 = {}", str1);

    mDs.get().printUnknownData(4, fmt::format("{}: 0", __func__));

    const uint16_t len0 = mDs.get().readUint16();

    for(size_t i = 0; i < len0; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();
        readPrimitive(primitive);
    }

    mDs.get().printUnknownData(8, fmt::format("{}: 1", __func__));

    sanitizeThisFutureSize(thisFuture);

    // readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}