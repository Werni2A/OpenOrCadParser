#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSthInHierarchy2.hpp"


void StructSthInHierarchy2::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    mDs.get().printUnknownData(4, fmt::format("{}: 0", __func__));

    const uint16_t val = mDs.get().readUint16();

    spdlog::trace("val = {}", val);

    // Try parsing either 0 or 10 bytes until the next prefix occurs
    {
        size_t currOffset = mDs.get().getCurrentOffset();

        bool success = false;

        if(!success)
        {
            try
            {
                auto_read_prefixes();
                readPreamble();
                success = true;

                // Restore previous state and parse bytes
                mDs.get().setCurrentOffset(currOffset);
            }
            catch(...)
            {
                mDs.get().setCurrentOffset(currOffset);
            }
        }

        if(!success)
        {
            try
            {
                mDs.get().printUnknownData(10, fmt::format("{}: 1", __func__));
                auto_read_prefixes();
                readPreamble();
                success = true;

                // Restore previous state and parse bytes
                mDs.get().setCurrentOffset(currOffset);
                mDs.get().printUnknownData(10, fmt::format("{}: 1", __func__));
            }
            catch(...)
            {
                mDs.get().setCurrentOffset(currOffset);
            }
        }

        if(!success)
        {
            throw std::runtime_error("Unfortunate...");
        }
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}