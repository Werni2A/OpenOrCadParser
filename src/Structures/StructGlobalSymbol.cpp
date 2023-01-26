#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructGlobalSymbol.hpp"
#include "Structures/StructPrimitives.hpp"


void StructGlobalSymbol::read(FileFormatVersion /* aVersion */)
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

    mDs.get().printUnknownData(8);

    sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}