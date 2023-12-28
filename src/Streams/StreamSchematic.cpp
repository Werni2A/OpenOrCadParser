#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "GenericParser.hpp"
#include "Streams/StreamSchematic.hpp"


void StreamSchematic::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    // @todo Extract in separate structure parser
    parser.auto_read_prefixes(Structure::SchLib, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    // -----------------------------------------------

    const std::string schematic_name = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("schematic_name = {}", schematic_name);

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 1");

    const uint16_t schematicPages = ds.readUint16();

    mCtx.mLogger.trace("schematicPages = {}", schematicPages);

    for(size_t i = 0u; i < schematicPages; ++i)
    {
        const std::string page_name = ds.readStringLenZeroTerm();
        mCtx.mLogger.trace("page_name = {}", page_name);
    }

    const uint16_t len = ds.readUint16();

    mCtx.mLogger.trace("len = {}", len);

    for(size_t i = 0u; i < len; ++i)
    {
        ds.printUnknownData(4, getMethodName(this, __func__) + ": 1");
    }

    const uint16_t len2 = ds.readUint16();

    mCtx.mLogger.trace("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        ds.printUnknownData(5, getMethodName(this, __func__) + ": 2");
    }

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 3");

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}