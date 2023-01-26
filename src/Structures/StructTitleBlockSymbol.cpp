#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructPrimitives.hpp"
#include "Structures/StructTitleBlockSymbol.hpp"


void StructTitleBlockSymbol::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    // @todo Use readStructure()
    StructPrimitives primitives{mDs};
    primitives.read();

    mDs.get().printUnknownData(4, fmt::format("{}: 0", __func__));

    const uint16_t len0 = mDs.get().readUint16();

    for(size_t i = 0; i < len0; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();
        readPrimitive(primitive);
    }

    mDs.get().printUnknownData(8, fmt::format("{}: 1", __func__));

    sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}