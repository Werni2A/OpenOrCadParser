#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructT0x10.hpp"


void StructT0x10::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    const uint16_t sth = mDs.get().readUint16();

    spdlog::debug("sth = {}", sth);

    const int16_t pointX = mDs.get().readInt16();
    const int16_t pointY = mDs.get().readInt16();

    spdlog::debug("pointX = {}", pointX);
    spdlog::debug("pointY = {}", pointY);

    const uint32_t maybeId = mDs.get().readUint32();

    spdlog::debug("maybeId = {}", maybeId);

    const uint32_t unkownInt = mDs.get().readUint32();

    spdlog::debug("unkownInt = {}", unkownInt);

    // Should be Display Properties
    uint16_t len = mDs.get().readUint16();

    spdlog::debug("len = {}", len);

    for(size_t i = 0; i < len; ++i)
    {
        readStructure();
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}