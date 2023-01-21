#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructT0x34.hpp"


void StructT0x34::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    // auto_read_prefixes();

    // readPreamble();

    // @todo Looks like Primitive types!
    //     0 Byte = structure id
    // 1 - 4 Byte = struct len
    // 5 - 8 Byte = zero vector
    mDs.get().printUnknownData(9, std::string(__func__) + " - 0");

    const std::optional<FutureData> thisFuture = getFutureData();

    id = mDs.get().readUint32();

    spdlog::debug("id = {}", id);

    const std::string unknownStr = mDs.get().readStringLenZeroTerm();

    spdlog::debug("unknownStr = {}", unknownStr);

    const uint32_t unknownInt = mDs.get().readUint32();

    spdlog::debug("unknownInt = {}", unknownInt);

    color = ToColor(mDs.get().readUint32());

    spdlog::debug("color = {}", ::to_string(color));

    lineStyle = ToLineStyle(mDs.get().readUint32());

    spdlog::debug("lineStyle = {}", ::to_string(lineStyle));

    lineWidth = ToLineWidth(mDs.get().readUint32());

    spdlog::debug("lineWidth = {}", ::to_string(lineWidth));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}