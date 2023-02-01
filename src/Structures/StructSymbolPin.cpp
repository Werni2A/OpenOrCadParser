#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/PortType.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "PinShape.hpp"
#include "Structures/StructSymbolPin.hpp"


void StructSymbolPin::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    FutureDataLst localFutureLst{mDs};

    auto_read_prefixes({Structure::SymbolPinBus, Structure::SymbolPinScalar}, localFutureLst);

    readPreamble();

    name = mDs.get().readStringLenZeroTerm();

    startX = mDs.get().readInt32();
    startY = mDs.get().readInt32();
    hotptX = mDs.get().readInt32();
    hotptY = mDs.get().readInt32();

    pinShape = ToPinShape(mDs.get().readUint16());

    mDs.get().printUnknownData(2, getMethodName(this, __func__) + ": 0");

    portType = ToPortType(mDs.get().readUint32());

    mDs.get().printUnknownData(4, getMethodName(this, __func__) + ": 1");

    const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}