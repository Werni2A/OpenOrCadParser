#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Color.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructPlacedInstance.hpp"


void StructPlacedInstance::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::PlacedInstance, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    ds.printUnknownData(8, getMethodName(this, __func__) + ": 0");

    const std::string pkgName = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("pkgName = {}", pkgName);

    const uint32_t dbId = ds.readUint32();

    mCtx.mLogger.trace("dbId = {}", dbId);

    ds.printUnknownData(8, getMethodName(this, __func__) + ": 1");

    const int16_t locX = ds.readInt16();
    const int16_t locY = ds.readInt16();

    mCtx.mLogger.trace("locX = {}", locX);
    mCtx.mLogger.trace("locY = {}", locY);

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 2");

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    mCtx.mLogger.trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(parser.readStructure()));
    }

    ds.printUnknownData(1, getMethodName(this, __func__) + ": 3");

    localFutureLst.checkpoint();

    const std::string reference = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("reference = {}", reference);

    ds.printUnknownData(14, getMethodName(this, __func__) + ": 4");

    const uint16_t lenT0x10s = ds.readUint16();

    mCtx.mLogger.trace("lenT0x10s = {}", lenT0x10s);

    for(size_t i = 0u; i < lenT0x10s; ++i)
    {
        t0x10s.push_back(dynamic_pointer_cast<StructT0x10>(parser.readStructure()));
    }

    localFutureLst.checkpoint();

    const std::string source_package = ds.readStringLenZeroTerm(); // @todo needs verification

    mCtx.mLogger.trace("source_package = {}", source_package);

    ds.printUnknownData(2, getMethodName(this, __func__) + ": 5");

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}