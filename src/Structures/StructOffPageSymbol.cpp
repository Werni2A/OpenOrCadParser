#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructOffPageSymbol.hpp"
#include "Structures/StructPrimitives.hpp"


void StructOffPageSymbol::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.get().getCurrentOffset()));

    // const std::optional<FutureData> thisFuture = getFutureData();

    StructPrimitives primitives{mDs};
    primitives.read();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}