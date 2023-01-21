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

    auto_read_prefixes();

    // const std::optional<FutureData> thisFuture = getFutureData();

    const auto readSmallPrefixPrimitive = [&, this]() -> Primitive
        {
            Primitive primitive = ToPrimitive(mDs.get().readUint8());
            mDs.get().assumeData({0x00}, std::string(__func__) + " - 0");
            mDs.get().assumeData({static_cast<uint8_t>(primitive)}, std::string(__func__) + " - 1");

            return primitive;
        };

    // mDs.get().printUnknownData(20, std::string(__func__) + " - x");
    // read_type_prefix();

    // @todo Figure out the size
    //       Probably it's always 20 Byte, try this first
    // discard_until_preamble();

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
    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00, 0x02, 0x00}, std::string(__func__) + " - 2");
    // mDs.get().printUnknownData(12, std::string(__func__) + " - 2");

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}