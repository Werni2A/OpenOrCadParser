#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructOffPageSymbol.hpp"


void StructOffPageSymbol::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    auto_read_prefixes(Structure::OffPageSymbol);

    readPreamble();

    const std::string name = mDs.get().readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    const std::string someStr = mDs.get().readStringLenZeroTerm();

    spdlog::trace("someStr = {}", someStr);

    mDs.get().printUnknownData(4, fmt::format("{}: 0", getMethodName(this, __func__)));

    const uint16_t len0 = mDs.get().readUint16();

    for(size_t i = 0; i < len0; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();
        readPrimitive(primitive);
    }

    mDs.get().printUnknownData(8);

    sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}