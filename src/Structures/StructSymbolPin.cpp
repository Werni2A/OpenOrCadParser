#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/PortType.hpp"
#include "General.hpp"
#include "PinShape.hpp"
#include "Structures/StructSymbolPin.hpp"


// void StructSymbolPin::read(FileFormatVersion /* aVersion */)
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