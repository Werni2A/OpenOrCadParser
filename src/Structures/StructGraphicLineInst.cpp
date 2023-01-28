#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructGraphicLineInst.hpp"


void StructGraphicLineInst::read(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion();
    }

    auto_read_prefixes(Structure::GraphicLineInst);

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    mDs.get().printUnknownData(34, getMethodName(this, __func__) + ": 0");

    sthInPages0 = dynamic_pointer_cast<StructSthInPages0>(readStructure());

    // @todo I don't know about the exact file format version.
    //       `C` was just chosen to get it running.
    if(aVersion == FileFormatVersion::C)
    {
        mDs.get().printUnknownData(8, getMethodName(this, __func__) + ": 1");
    }
    else
    {
        mDs.get().printUnknownData(16, getMethodName(this, __func__) + ": 1");
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}