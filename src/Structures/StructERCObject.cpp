#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Container.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructERCObject.hpp"


void StructERCObject::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::ERCObject, localFutureLst);

    StructGraphicInst::read(localFutureLst);

    s0 = ds.readStringLenZeroTerm();
    s1 = ds.readStringLenZeroTerm();
    s2 = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("s0 = {}", s0);
    mCtx.mLogger.trace("s1 = {}", s1);
    mCtx.mLogger.trace("s2 = {}", s2);

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}