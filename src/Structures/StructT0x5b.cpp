#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructT0x5b.hpp"

void StructT0x5b::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::T0x5b, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}