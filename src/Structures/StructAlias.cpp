#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructAlias.hpp"

void OOCP::StructAlias::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::Alias, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    locX = ds.readInt32();
    locY = ds.readInt32();

    mCtx.mLogger.trace("locX = {}", locX);
    mCtx.mLogger.trace("locY = {}", locY);

    color = ToColor(ds.readUint32());

    mCtx.mLogger.trace("color = {}", OOCP::to_string(color));

    rotation = ToRotation(ds.readUint32()); // @todo Why is it 4 byte? Probably increase Rotation size

    mCtx.mLogger.trace("rotation = {}", OOCP::to_string(rotation));

    uint32_t textFontIdx = ds.readUint32();

    mCtx.mLogger.trace("Alias fontIdx = {}", textFontIdx);

    name = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("name = {}", name);

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}