#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Primitives/PrimSymbolVector.hpp"


void PrimSymbolVector::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const auto readSmallPrefixPrimitive = [&, this]() -> Primitive
        {
            Primitive primitive = ToPrimitive(mDs.get().readUint8());
            mDs.get().assumeData({0x00}, getMethodName(this, __func__) + ": 0");
            mDs.get().assumeData({static_cast<uint8_t>(primitive)}, getMethodName(this, __func__) + ": 1");

            return primitive;
        };

    FutureDataLst localFutureLst{mDs};

    auto_read_prefixes(localFutureLst);

    readPreamble();

    locX = mDs.get().readInt16();
    locY = mDs.get().readInt16();

    const uint16_t repetition = mDs.get().readUint16();

    // Only primitives are expected here
    for(size_t i = 0u; i < repetition; ++i)
    {
        const Primitive primitive = readSmallPrefixPrimitive();

        readPrimitive(primitive);
    }

    name = mDs.get().readStringLenZeroTerm();

    // @todo contains smallPrefixPrimitive
    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00, 0x02, 0x00}, getMethodName(this, __func__) + ": 2");
    // mDs.get().printUnknownData(12, getMethodName(this, __func__) + ": 2");


    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}