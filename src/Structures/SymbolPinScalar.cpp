#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "../Parser.hpp"
#include "SymbolPinScalar.hpp"


SymbolPinScalar Parser::readSymbolPinScalar()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    SymbolPinScalar symbolPinScalar;

    symbolPinScalar.name = mDs.readStringLenZeroTerm();

    symbolPinScalar.startX = mDs.readInt32();
    symbolPinScalar.startY = mDs.readInt32();
    symbolPinScalar.hotptX = mDs.readInt32();
    symbolPinScalar.hotptY = mDs.readInt32();

    symbolPinScalar.pinShape = ToPinShape(mDs.readUint16());

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    symbolPinScalar.portType = ToPortType(mDs.readUint32());

    mDs.printUnknownData(6, std::string(__func__) + " - 1");

    sanitizeThisFutureSize(thisFuture);

    const std::optional<FutureData> nextFuture = checkTrailingFuture();

    if(nextFuture.has_value())
    {
        spdlog::warn("Detected trailing future data at 0x{:08x}", nextFuture.value().getStartOffset());
        mDs.printUnknownData(nextFuture.value().getByteLen(), fmt::format("{}: Trailing Data", __func__));
    }

    sanitizeThisFutureSize(nextFuture);

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(symbolPinScalar));

    return symbolPinScalar;
}