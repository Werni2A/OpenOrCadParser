#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/TrailingProperties.hpp"


TrailingProperties Parser::readTrailingProperties()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    TrailingProperties obj;

    // @todo use enum for the view (normal/convert)
    const uint16_t viewNumber = mDs.readUint16(); // @todo I assume that this is the amount of views
                                               // the symbol has. Typically 1 (.Normal) or maybe
                                               // 2 with (.Normal and .Convert)
                                               // @todo Add to obj

    spdlog::debug("viewNumber = {}", viewNumber);

    if(viewNumber == 1U) // Contains ".Normal"
    {
        obj.normalName = mDs.readStringLenZeroTerm();
    }

    if(viewNumber == 2U) // Contains ".Normal" and ".Convert"
    {
        obj.normalName = mDs.readStringLenZeroTerm();
        obj.convertName = mDs.readStringLenZeroTerm();
    }

    spdlog::debug("normalName  = {}", obj.normalName);
    spdlog::debug("convertName = {}", obj.convertName);

    if(viewNumber != 1U && viewNumber != 2U)
    {
        const std::string msg = fmt::format("viewNumber = {} but expected it to be 1 or 2!",
            viewNumber);

        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}