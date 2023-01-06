#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamHierarchy.hpp"


void StreamHierarchy::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.get().getCurrentOffset()));

    mDs.get().printUnknownData(9, std::string(__func__) + " - 0");

    std::string schematicName = mDs.get().readStringLenZeroTerm();

    mDs.get().printUnknownData(9, std::string(__func__) + " - 1");

    const uint16_t netLen = mDs.get().readUint16();

    for(size_t i = 0u; i < netLen; ++i)
    {
        // const Structure structure = read_prefixes(2);
        Structure structure = auto_read_prefixes();

        readPreamble();

        // @todo Move the following data into a own structure for the specific Structure type

        uint32_t dbId = mDs.get().readUint32();

        std::string name = mDs.get().readStringLenZeroTerm(); // net name
    }

    spdlog::debug(getClosingMsg(__func__, mDs.get().getCurrentOffset()));
    spdlog::debug(to_string());
}