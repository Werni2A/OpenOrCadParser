#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructT0x35.hpp"


void StructT0x35::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    // auto_read_prefixes(Structure::T0x35);

    // readPreamble();

    // @todo Looks like Primitive types!
    //     0 Byte = structure id
    // 1 - 4 Byte = struct len
    // 5 - 8 Byte = zero vector
    mDs.get().printUnknownData(9, fmt::format("{}: 0", __func__));

    const std::optional<FutureData> thisFuture = getFutureData();

    id = mDs.get().readUint32();

    spdlog::trace("id = {}", id);

    std::string unknownStr = mDs.get().readStringLenZeroTerm();

    spdlog::trace("unknownStr = {}", unknownStr);

    uint32_t unknownInt = mDs.get().readUint32();

    spdlog::trace("unknownInt = {}", unknownInt);

    color = ToColor(mDs.get().readUint32());

    spdlog::trace("color = {}", ::to_string(color));

    lineStyle = ToLineStyle(mDs.get().readUint32());

    spdlog::trace("lineStyle = {}", ::to_string(lineStyle));

    lineWidth = ToLineWidth(mDs.get().readUint32());

    spdlog::trace("lineWidth = {}", ::to_string(lineWidth));

    const uint32_t len0 = mDs.get().readUint16();

    spdlog::trace("len0 = {}", len0);

    for(size_t i = 0; i < len0; ++i)
    {
        mDs.get().printUnknownData(4, fmt::format("{}: 1 - {}", __func__, i));
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}