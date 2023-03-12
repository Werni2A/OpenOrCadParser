#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructERCSymbolInst.hpp"


void StructERCSymbolInst::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::ERCSymbolInst, localFutureLst);

    StructGraphicInst::read(localFutureLst);

    s0 = ds.readStringLenZeroTerm();
    s1 = ds.readStringLenZeroTerm();
    s2 = ds.readStringLenZeroTerm();

    spdlog::trace("s0 = {}", s0);
    spdlog::trace("s1 = {}", s1);
    spdlog::trace("s2 = {}", s2);

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}