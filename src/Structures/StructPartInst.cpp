#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Color.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructPartInst.hpp"


void StructPartInst::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::PartInst, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    ds.printUnknownData(8, getMethodName(this, __func__) + ": 0");

    const std::string pkgName = ds.readStringLenZeroTerm();

    spdlog::trace("pkgName = {}", pkgName);

    const uint32_t dbId = ds.readUint32();

    spdlog::trace("dbId = {}", dbId);

    ds.printUnknownData(8, getMethodName(this, __func__) + ": 1");

    const int16_t locX = ds.readInt16();
    const int16_t locY = ds.readInt16();

    spdlog::trace("locX = {}", locX);
    spdlog::trace("locY = {}", locY);

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 2");

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    ds.printUnknownData(1, getMethodName(this, __func__) + ": 3");

    localFutureLst.checkpoint();

    const std::string reference = ds.readStringLenZeroTerm();

    spdlog::trace("reference = {}", reference);

    ds.printUnknownData(14, getMethodName(this, __func__) + ": 4");

    const uint16_t lenT0x10s = ds.readUint16();

    spdlog::trace("lenT0x10s = {}", lenT0x10s);

    for(size_t i = 0u; i < lenT0x10s; ++i)
    {
        t0x10s.push_back(dynamic_pointer_cast<StructT0x10>(readStructure()));
    }

    localFutureLst.checkpoint();

    const std::string sth1 = ds.readStringLenZeroTerm(); // @todo needs verification

    spdlog::trace("sth1 = {}", sth1);

    ds.printUnknownData(2, getMethodName(this, __func__) + ": 5");

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}