#include <cmath>
#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/PortType.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "PinShape.hpp"
#include "Structures/StructSymbolPin.hpp"


void StructSymbolPin::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};


    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes({Structure::SymbolPinBus, Structure::SymbolPinScalar}, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    name = ds.readStringLenZeroTerm();

    startX = ds.readInt32();
    startY = ds.readInt32();
    hotptX = ds.readInt32();
    hotptY = ds.readInt32();

    pinShape = ToPinShape(ds.readUint16(), mCtx.mLogger);

    ds.printUnknownData(2, getMethodName(this, __func__) + ": 0");

    portType = ToPortType(ds.readUint32());

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 1");

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    mCtx.mLogger.trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(parser.readStructure()));
    }

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}


int32_t StructSymbolPin::getPinLength() const
{
    const int32_t dx = hotptX - startX;
    const int32_t dy = hotptY - startY;

    int32_t pinLength = static_cast<int32_t>(std::round(std::sqrt(dx * dx + dy * dy)));

    if(!pinShape.isLong)
    {
        pinLength /= 2;
    }

    if(ToShapeType(pinShape) == ShapeType::ZeroLength)
    {
        pinLength = 0;
    }

    return pinLength;
}