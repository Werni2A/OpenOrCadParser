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
void StructSthInPages0::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::SthInPages0, localFutureLst);

    readPreamble();

    ds.printUnknownData(6, getMethodName(this, __func__) + ": 0");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 1");

    const uint16_t len = ds.readUint16();

    spdlog::trace("len = {}", len);

    for(size_t i = 0u; i < len; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();

        readPrimitive(primitive);
    }

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}