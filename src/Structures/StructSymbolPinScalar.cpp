#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructSymbolPinScalar.hpp"


StructSymbolPinScalar Parser::readStructSymbolPinScalar()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructSymbolPinScalar obj;

    obj.name = mDs.readStringLenZeroTerm();

    obj.startX = mDs.readInt32();
    obj.startY = mDs.readInt32();
    obj.hotptX = mDs.readInt32();
    obj.hotptY = mDs.readInt32();

    obj.pinShape = ToPinShape(mDs.readUint16());

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    obj.portType = ToPortType(mDs.readUint32());

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
    spdlog::info(to_string(obj));

    return obj;
}