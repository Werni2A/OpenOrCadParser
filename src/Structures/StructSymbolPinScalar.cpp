#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructSymbolPinScalar.hpp"


void StructSymbolPinScalar::read(FileFormatVersion /* aVersion */)
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

    mDs.get().printUnknownData(4, std::string(__func__) + " - 1");

    const uint16_t struct_len = mDs.get().readUint16();

    for(size_t i = 0U; i < struct_len; ++i)
    {
        readStructure();
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}