#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSymbol.hpp"


// void StructSymbol::read(FileFormatVersion /* aVersion */)
// {
//     spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

//     auto_read_prefixes();

//     readPreamble();

//     const std::optional<FutureData> thisFuture = getFutureData();



//     sanitizeThisFutureSize(thisFuture);

//     readOptionalTrailingFuture();

//     spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
//     spdlog::trace(to_string());
// }