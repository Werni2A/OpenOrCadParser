#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructT0x1f.hpp"


// @todo Probably specifies the 'Package Properties'
void StructT0x1f::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::T0x1f, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    name = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("name = {}", name);

    std::string unknownStr0 = ds.readStringLenZeroTerm(); // @todo figure out
    mCtx.mLogger.trace("{}: unknownStr0 = {}", __func__, unknownStr0);

    localFutureLst.checkpoint();

    refDes = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("refDes = {}", refDes);

    std::string unknownStr1 = ds.readStringLenZeroTerm(); // @todo figure out
    mCtx.mLogger.trace("{}: unknownStr1 = {}", __func__, unknownStr1);

    pcbFootprint = ds.readStringLenZeroTerm();

    const uint16_t lenPinIdxMappings = ds.readUint16();

    mCtx.mLogger.trace("lenPinIdxMappings = {}", lenPinIdxMappings);

    for(size_t i = 0u; i < lenPinIdxMappings; ++i)
    {
        pinIdxMappings.push_back(dynamic_pointer_cast<StructPinIdxMapping>(parser.readStructure()));
    }

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}