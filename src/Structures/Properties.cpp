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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}