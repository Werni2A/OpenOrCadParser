#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Primitives/PrimSymbolVector.hpp"


void PrimSymbolVector::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const auto readSmallPrefixPrimitive = [&, this]() -> Primitive
        {
            Primitive primitive = ToPrimitive(ds.readUint8());
            ds.assumeData({0x00}, getMethodName(this, __func__) + ": 0");
            ds.assumeData({static_cast<uint8_t>(primitive)}, getMethodName(this, __func__) + ": 1");

            return primitive;
        };

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(localFutureLst);

    parser.readPreamble();

    locX = ds.readInt16();
    locY = ds.readInt16();

    const uint16_t lenPrimitives = ds.readUint16();

    spdlog::trace("lenPrimitives = {}", lenPrimitives);

    for(size_t i = 0u; i < lenPrimitives; ++i)
    {
        const Primitive primitive = readSmallPrefixPrimitive();

        parser.readPrimitive(primitive);
    }

    name = ds.readStringLenZeroTerm();

    // @todo contains smallPrefixPrimitive
    // ds.assumeData({0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00, 0x02, 0x00}, getMethodName(this, __func__) + ": 2");
    // ds.printUnknownData(12, getMethodName(this, __func__) + ": 2");


    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}