#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "Structures/StructSthInPages0.hpp"


// @todo Probably a wrapper for Inst (Instances)
void StructSthInPages0::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion();
    }

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::SthInPages0, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    ds.printUnknownData(6, getMethodName(this, __func__) + ": 0");

    localFutureLst.checkpoint();

    color = ToColor(ds.readUint32());

    spdlog::trace("color = {}", ::to_string(color));

    const uint16_t lenPrimitives = ds.readUint16();

    spdlog::trace("lenPrimitives = {}", lenPrimitives);

    for(size_t i = 0u; i < lenPrimitives; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();

        readPrimitive(primitive);
    }

    // @todo The versions were chosen arbitrarily
    if(aVersion == FileFormatVersion::C)
    {
        ds.printUnknownData(16, getMethodName(this, __func__) + ": 2");
    }
    else if(aVersion == FileFormatVersion::B)
    {
        ds.printUnknownData(8, getMethodName(this, __func__) + ": 3");
    }
    else
    { }

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}