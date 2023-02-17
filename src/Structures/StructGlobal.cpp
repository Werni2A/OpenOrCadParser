#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructGlobal.hpp"


void StructGlobal::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::Global, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    ds.printUnknownData(8, fmt::format("{}: 0", getMethodName(this, __func__)));

    const std::string name = ds.readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    ds.printUnknownData(20, fmt::format("{}: 1", getMethodName(this, __func__)));

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    ds.printUnknownData(1, fmt::format("{}: 2", getMethodName(this, __func__)));

    localFutureLst.readUntilNextFutureData("See FuturData of StructGlobal");

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}