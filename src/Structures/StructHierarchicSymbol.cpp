#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructHierarchicSymbol.hpp"
#include "Structures/StructPrimitives.hpp"


void StructHierarchicSymbol::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    // const std::optional<FutureData> thisFuture = getFutureData();

    StructPrimitives primitives{mDs};
    primitives.read();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}