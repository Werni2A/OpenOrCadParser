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
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes({Structure::SymbolPinBus, Structure::SymbolPinScalar}, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    name = ds.readStringLenZeroTerm();

    startX = ds.readInt32();
    startY = ds.readInt32();
    hotptX = ds.readInt32();
    hotptY = ds.readInt32();

    pinShape = ToPinShape(ds.readUint16());

    ds.printUnknownData(2, getMethodName(this, __func__) + ": 0");

    portType = ToPortType(ds.readUint32());

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 1");

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    localFutureLst.readUntilNextFutureData("See FuturData of StructSymbolPin");

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}