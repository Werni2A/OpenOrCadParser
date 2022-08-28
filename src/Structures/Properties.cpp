#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "../Parser.hpp"
#include "Properties.hpp"


Properties Parser::readProperties()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    Properties obj;

    obj.ref = mDs.readStringLenZeroTerm();

    // @todo Probably a string
    mDs.assumeData({0x00, 0x00, 0x00}, fmt::format("{}: 0", __func__));

    sanitizeThisFutureSize(thisFuture);

    if(checkTrailingFuture().has_value())
    {
        // @todo save returned structure as optional in Properties or in outter structure?
        readPropertiesTrailing();
    }

    // @todo this belongs to the outter structure. Move it out of this parser function
    mDs.printUnknownData( 2, fmt::format("{}: 1", __func__)); // @todo this is probably a length specifying the number of trailing structures
    mDs.printUnknownData(27, fmt::format("{}: 2", __func__)); // @todo This is 3x read_single_prefix()

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}