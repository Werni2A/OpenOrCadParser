#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructSthInHierarchy2.hpp"


StructSthInHierarchy2 Parser::readStructSthInHierarchy2()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructSthInHierarchy2 obj;

    mDs.printUnknownData(4, fmt::format("{}: 0", __func__));

    const uint16_t val = mDs.readUint16();

    spdlog::debug("val = {}", val);

    // Try parsing either 0 or 10 bytes until the next prefix occurs
    {
        size_t currOffset = mDs.getCurrentOffset();

        bool success = false;

        if(!success)
        {
            try
            {
                auto_read_prefixes();
                readPreamble();
                success = true;

                // Restore previous state and parse bytes
                mDs.setCurrentOffset(currOffset);
            }
            catch(...)
            {
                mDs.setCurrentOffset(currOffset);
            }
        }

        if(!success)
        {
            try
            {
                mDs.printUnknownData(10, fmt::format("{}: 1", __func__));
                auto_read_prefixes();
                readPreamble();
                success = true;

                // Restore previous state and parse bytes
                mDs.setCurrentOffset(currOffset);
                mDs.printUnknownData(10, fmt::format("{}: 1", __func__));
            }
            catch(...)
            {
                mDs.setCurrentOffset(currOffset);
            }
        }

        if(!success)
        {
            throw std::runtime_error("Unfortunate...");
        }
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}