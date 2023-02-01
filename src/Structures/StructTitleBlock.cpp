#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructTitleBlock.hpp"


void StructTitleBlock::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    FutureDataLst localFutureLst{mDs};

    auto_read_prefixes(Structure::TitleBlock, localFutureLst);

    readPreamble();

    mDs.get().printUnknownData(8, fmt::format("{}: 0", getMethodName(this, __func__)));

    const std::string foo = mDs.get().readStringLenZeroTerm();

    spdlog::trace("foo = {}", foo);

    mDs.get().printUnknownData(20, fmt::format("{}: 1", getMethodName(this, __func__)));

    const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    mDs.get().printUnknownData(13, fmt::format("{}: 2", getMethodName(this, __func__)));

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}