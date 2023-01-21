#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructWireBus.hpp"


void StructWireBus::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    mDs.get().printUnknownData(4, fmt::format("{}: 0", __func__));

    id = mDs.get().readUint32();

    spdlog::debug("id = {}", id);

    color = ToColor(mDs.get().readUint32());

    spdlog::debug("color = {}", ::to_string(color));

    startX = mDs.get().readInt32();
    startY = mDs.get().readInt32();

    spdlog::debug("startX = {}", startX);
    spdlog::debug("startY = {}", startY);

    endX = mDs.get().readInt32();
    endY = mDs.get().readInt32();

    spdlog::debug("endX = {}", endX);
    spdlog::debug("endY = {}", endY);

    mDs.get().printUnknownData(1, fmt::format("{}: 1", __func__));

    // Should be Alias
    const uint16_t len0 = mDs.get().readUint16();

    spdlog::debug("len0 = {}", len0);

    for(size_t i = 0; i < len0; ++i)
    {
        readStructure();
    }

    // Should be DisplayProperty
    const uint16_t len1 = mDs.get().readUint16();

    spdlog::debug("len1 = {}", len1);

    for(size_t i = 0; i < len1; ++i)
    {
        readStructure();
    }

    lineWidth = ToLineWidth(mDs.get().readUint32());

    spdlog::debug("lineWidth = {}", ::to_string(lineWidth));

    lineStyle = ToLineStyle(mDs.get().readUint32());

    spdlog::debug("lineStyle = {}", ::to_string(lineStyle));

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}