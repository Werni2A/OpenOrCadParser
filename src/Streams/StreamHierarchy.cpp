#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamHierarchy.hpp"


void StreamHierarchy::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    ds.printUnknownData(9, getMethodName(this, __func__) + ": 0");

    const std::string schematicName = ds.readStringLenZeroTerm();

    spdlog::trace("schematicName = {}", schematicName);

    ds.printUnknownData(9, getMethodName(this, __func__) + ": 1");

    const uint16_t netLen = ds.readUint16();

    spdlog::trace("netLen = {}", netLen);

    for(size_t i = 0u; i < netLen; ++i)
    {
        FutureDataLst localFutureLst{mCtx};

        auto_read_prefixes(localFutureLst);

        readPreamble();

        localFutureLst.readRestOfStructure();

        // @todo Move the following data into its own structure for the specific Structure type

        const uint32_t dbId = ds.readUint32();

        spdlog::trace("dbId = {}", dbId);

        const std::string name = ds.readStringLenZeroTerm(); // net name

        spdlog::trace("name = {}", name);
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}