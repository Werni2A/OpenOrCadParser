#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/PortType.hpp"
#include "General.hpp"
#include "PinShape.hpp"
#include "Structures/StructSymbolPinBus.hpp"


void StructSymbolPinBus::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    name = mDs.get().readStringLenZeroTerm();

    startX = mDs.get().readInt32();
    startY = mDs.get().readInt32();
    hotptX = mDs.get().readInt32();
    hotptY = mDs.get().readInt32();

    pinShape = ToPinShape(mDs.get().readUint16());

    mDs.get().printUnknownData(2, std::string(__func__) + " - 0");

    portType = ToPortType(mDs.get().readUint32());

    mDs.get().printUnknownData(6, std::string(__func__) + " - 1");

    // @todo compare against SymbolPinScalar, maybe they have the same content

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}