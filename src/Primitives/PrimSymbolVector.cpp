#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Primitives/PrimSymbolVector.hpp"


PrimSymbolVector Parser::readPrimSymbolVector()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    PrimSymbolVector obj;

    const auto readSmallPrefixPrimitive = [&, this]() -> Primitive
        {
            Primitive primitive = ToPrimitive(mDs.readUint8());
            mDs.assumeData({0x00}, std::string(__func__) + " - 0");
            mDs.assumeData({static_cast<uint8_t>(primitive)}, std::string(__func__) + " - 1");

            return primitive;
        };

    // mDs.printUnknownData(20, std::string(__func__) + " - x");
    // read_type_prefix();

    // @todo Figure out the size
    //       Probably it's always 20 Byte, try this first
    discard_until_preamble();
    readPreamble();

    obj.locX = mDs.readInt16();
    obj.locY = mDs.readInt16();

    const uint16_t repetition = mDs.readUint16();

    // Only primitives are expected here
    for(size_t i = 0u; i < repetition; ++i)
    {

        const Primitive primitive = readSmallPrefixPrimitive();

        readPrimitive(primitive);
    }

    obj.name = mDs.readStringLenZeroTerm();

    // @todo contains smallPrefixPrimitive
    mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00, 0x02, 0x00}, std::string(__func__) + " - 2");
    // mDs.printUnknownData(12, std::string(__func__) + " - 2");

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}