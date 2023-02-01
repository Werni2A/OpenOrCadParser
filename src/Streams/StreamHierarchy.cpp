#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamHierarchy.hpp"


void StreamHierarchy::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    mDs.get().printUnknownData(9, getMethodName(this, __func__) + ": 0");

    const std::string schematicName = mDs.get().readStringLenZeroTerm();

    spdlog::trace("schematicName = {}", schematicName);

    mDs.get().printUnknownData(9, getMethodName(this, __func__) + ": 1");

    const uint16_t netLen = mDs.get().readUint16();

    spdlog::trace("netLen = {}", netLen);

    for(size_t i = 0u; i < netLen; ++i)
    {
        FutureDataLst localFutureLst{mDs};

        auto_read_prefixes(localFutureLst);

        readPreamble();

        localFutureLst.readRestOfStructure();

        // @todo Move the following data into its own structure for the specific Structure type

        const uint32_t dbId = mDs.get().readUint32();

        spdlog::trace("dbId = {}", dbId);

        const std::string name = mDs.get().readStringLenZeroTerm(); // net name

        spdlog::trace("name = {}", name);
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}