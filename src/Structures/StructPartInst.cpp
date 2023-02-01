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
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    FutureDataLst localFutureLst{mDs};

    auto_read_prefixes(Structure::PartInst, localFutureLst);

    readPreamble();

    mDs.get().printUnknownData(8, getMethodName(this, __func__) + ": 0");

    const std::string pkgName = mDs.get().readStringLenZeroTerm();

    spdlog::trace("pkgName = {}", pkgName);

    const uint32_t dbId = mDs.get().readUint32();

    spdlog::trace("dbId = {}", dbId);

    mDs.get().printUnknownData(8, getMethodName(this, __func__) + ": 1");

    const int16_t locX = mDs.get().readInt16();
    const int16_t locY = mDs.get().readInt16();

    spdlog::trace("locX = {}", locX);
    spdlog::trace("locY = {}", locY);

    mDs.get().printUnknownData(4, getMethodName(this, __func__) + ": 2");

    const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    mDs.get().printUnknownData(1, getMethodName(this, __func__) + ": 3");

    const std::string reference = mDs.get().readStringLenZeroTerm();

    spdlog::trace("reference = {}", reference);

    mDs.get().printUnknownData(14, getMethodName(this, __func__) + ": 4");

    const uint16_t lenT0x10s = mDs.get().readUint16();

    spdlog::trace("lenT0x10s = {}", lenT0x10s);

    for(size_t i = 0u; i < lenT0x10s; ++i)
    {
        t0x10s.push_back(dynamic_pointer_cast<StructT0x10>(readStructure()));
    }

    const std::string sth1 = mDs.get().readStringLenZeroTerm(); // @todo needs verification

    spdlog::trace("sth1 = {}", sth1);

    mDs.get().printUnknownData(2, getMethodName(this, __func__) + ": 5");

    // @todo implement type_prefix_very_long
    // mDs.get().printUnknownData(18, getMethodName(this, __func__) + ": 6");

    // auto_read_prefixes();

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}