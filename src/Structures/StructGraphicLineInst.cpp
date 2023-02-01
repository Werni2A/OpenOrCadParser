#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructGraphicLineInst.hpp"


void StructGraphicLineInst::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion();
    }

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::GraphicLineInst, localFutureLst);

    readPreamble();

    ds.printUnknownData(34, getMethodName(this, __func__) + ": 0");

    sthInPages0 = dynamic_pointer_cast<StructSthInPages0>(readStructure());

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}