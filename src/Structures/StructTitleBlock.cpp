#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructTitleBlock.hpp"


void StructTitleBlock::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    mDs.get().printUnknownData(8, fmt::format("{}: 0", __func__));

    const std::string foo = mDs.get().readStringLenZeroTerm();

    spdlog::trace("foo = {}", foo);

    mDs.get().printUnknownData(20, fmt::format("{}: 1", __func__));

    const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    mDs.get().printUnknownData(11, fmt::format("{}: 2", __func__));

    const uint16_t len1 = mDs.get().readUint16();

    spdlog::trace("len1 = {}", len1);

    for(size_t i = 0; i < len1; ++i)
    {
        mDs.get().printUnknownData(32, fmt::format("{}: 3.{}", __func__, i));
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}