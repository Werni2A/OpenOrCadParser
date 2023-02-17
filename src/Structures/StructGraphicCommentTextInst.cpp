#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructGraphicCommentTextInst.hpp"


void StructGraphicCommentTextInst::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::GraphicCommentTextInst, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    ds.printUnknownData(34, getMethodName(this, __func__) + ": 0");

    sthInPages0 = dynamic_pointer_cast<StructSthInPages0>(readStructure());

    localFutureLst.readUntilNextFutureData("See FuturData of StructGraphicCommentTextInst");

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}