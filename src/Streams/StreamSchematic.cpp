#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamSchematic.hpp"


void StreamSchematic::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    // @todo extract into parseSchematic method that is called with structure

    const std::string schematic_name = mDs.get().readStringLenZeroTerm();

    spdlog::trace("schematic_name = {}", schematic_name);

    mDs.get().printUnknownData(4, getMethodName(this, __func__) + ": 1");

    const uint16_t schematicPages = mDs.get().readUint16();

    spdlog::trace("schematicPages = {}", schematicPages);

    for(size_t i = 0u; i < schematicPages; ++i)
    {
        const std::string page_name = mDs.get().readStringLenZeroTerm();
        spdlog::trace("page_name = {}", page_name);
    }

    const uint16_t len = mDs.get().readUint16();

    spdlog::trace("len = {}", len);

    for(size_t i = 0u; i < len; ++i)
    {
        mDs.get().printUnknownData(4, getMethodName(this, __func__) + ": 1");
    }

    const uint16_t len2 = mDs.get().readUint16();

    spdlog::trace("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        mDs.get().printUnknownData(5, getMethodName(this, __func__) + ": 2");
    }

    mDs.get().printUnknownData(4, getMethodName(this, __func__) + ": 3");

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}