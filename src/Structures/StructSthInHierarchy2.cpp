#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSthInHierarchy2.hpp"


void StructSthInHierarchy2::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    ds.printUnknownData(4, fmt::format("{}: 0", getMethodName(this, __func__)));

    const uint16_t val = ds.readUint16();

    spdlog::trace("val = {}", val);

    FutureDataLst localFutureLst{mCtx};

    // Try parsing either 0 or 10 bytes until the next prefix occurs
    {
        size_t currOffset = ds.getCurrentOffset();

        bool success = false;

        if(!success)
        {
            try
            {
                localFutureLst = FutureDataLst{mCtx};

                auto_read_prefixes(localFutureLst);

                readPreamble();

                localFutureLst.checkpoint();

                success = true;

                // Restore previous state and parse bytes
                ds.setCurrentOffset(currOffset);
            }
            catch(...)
            {
                ds.setCurrentOffset(currOffset);
            }
        }

        if(!success)
        {
            try
            {
                ds.printUnknownData(10, fmt::format("{}: 1", getMethodName(this, __func__)));

                localFutureLst = FutureDataLst{mCtx};

                auto_read_prefixes(localFutureLst);

                readPreamble();

                localFutureLst.checkpoint();

                success = true;

                // Restore previous state and parse bytes
                ds.setCurrentOffset(currOffset);
                ds.printUnknownData(10, fmt::format("{}: 2", getMethodName(this, __func__)));
            }
            catch(...)
            {
                ds.setCurrentOffset(currOffset);
            }
        }

        if(!success)
        {
            throw std::runtime_error("Unfortunate...");
        }
    }

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}