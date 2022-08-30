#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "../Parser.hpp"
#include "SymbolVector.hpp"


SymbolVector Parser::readSymbolVector()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    SymbolVector obj;

    const auto readSmallTypePrefix = [&, this]() -> GeometryStructure
        {
            GeometryStructure structure = ToGeometryStructure(mDs.readUint8());
            mDs.assumeData({0x00}, std::string(__func__) + " - 0");
            mDs.assumeData({static_cast<uint8_t>(structure)}, std::string(__func__) + " - 1");

            return structure;
        };

    // mDs.printUnknownData(20, std::string(__func__) + " - x");
    // read_type_prefix();

    discard_until_preamble();
    readPreamble();

    obj.locX = mDs.readInt16();
    obj.locY = mDs.readInt16();

    uint16_t repetition = mDs.readUint16();

    for(size_t i = 0u; i < repetition; ++i)
    {
        if(i > 0u)
        {
            readPreamble();
        }

        readGeometryStructure(readSmallTypePrefix());
    }

    readPreamble();
    obj.name = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00, 0x02, 0x00}, std::string(__func__) + " - 2");
    // mDs.printUnknownData(12, std::string(__func__) + " - 2");

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}