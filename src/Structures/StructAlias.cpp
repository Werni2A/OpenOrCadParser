#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructAlias.hpp"


void StructAlias::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::Alias, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    locX = ds.readInt32();
    locY = ds.readInt32();

    spdlog::trace("locX = {}", locX);
    spdlog::trace("locY = {}", locY);

    color = ToColor(ds.readUint32());

    spdlog::trace("color = {}", ::to_string(color));

    rotation = ToRotation(ds.readUint32()); // @todo Why is it 4 byte? Probably increase Rotation size

    spdlog::trace("rotation = {}", ::to_string(rotation));

    uint32_t textFontIdx = ds.readUint32();

    spdlog::trace("Alias fontIdx = {}", textFontIdx);

    name = ds.readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}