#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "../Parser.hpp"
#include "PropertiesTrailing.hpp"


PropertiesTrailing Parser::readPropertiesTrailing()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    PropertiesTrailing obj;

    // @todo use enum for the view (normal/convert)
    const uint16_t viewNumber = mDs.readUint16(); // @todo I assume that this is the amount of views
                                               // the symbol has. Typically 1 (.Normal) or maybe
                                               // 2 with (.Normal and .Convert)
                                               // @todo Add to obj

    spdlog::debug("viewNumber = {}", viewNumber);

    switch(viewNumber)
    {
        case 1: // ".Normal"
            obj.name = mDs.readStringLenZeroTerm();
            break;

        case 2: // ".Convert"
            // @todo how to handle optional attributes in my structures?
            obj.name = mDs.readStringLenZeroTerm();
            obj.convertName = mDs.readStringLenZeroTerm();

            spdlog::debug("convertName = {}", obj.convertName);
            break;

        default:
            throw std::runtime_error("viewNumber is " + std::to_string(viewNumber) +
                " but it was expected that this can only take the value 1 or 2!");
            break;
    }

    spdlog::debug("name = {}", obj.name);

    sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}