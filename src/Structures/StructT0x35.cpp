#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructT0x35.hpp"


void StructT0x35::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    // @todo Try to activate this part
    // FutureDataLst localFutureLst{mCtx};

    // ds.auto_read_prefixes(Structure::T0x35, localFutureLst);

    // ds.readPreamble();

    // @todo Looks like Primitive types!
    //     0 Byte = structure id
    // 1 - 4 Byte = struct len
    // 5 - 8 Byte = zero vector
    ds.printUnknownData(9, fmt::format("{}: 0", getMethodName(this, __func__)));

    id = ds.readUint32();

    spdlog::trace("id = {}", id);

    std::string unknownStr = ds.readStringLenZeroTerm();

    spdlog::trace("unknownStr = {}", unknownStr);

    uint32_t unknownInt = ds.readUint32();

    spdlog::trace("unknownInt = {}", unknownInt);

    color = ToColor(ds.readUint32());

    spdlog::trace("color = {}", ::to_string(color));

    lineStyle = ToLineStyle(ds.readUint32());

    spdlog::trace("lineStyle = {}", ::to_string(lineStyle));

    lineWidth = ToLineWidth(ds.readUint32());

    spdlog::trace("lineWidth = {}", ::to_string(lineWidth));

    const uint32_t len0 = ds.readUint16();

    spdlog::trace("len0 = {}", len0);

    for(size_t i = 0; i < len0; ++i)
    {
        ds.printUnknownData(4, fmt::format("{}: 1 - {}",
            getMethodName(this, __func__), i));
    }

    // localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}