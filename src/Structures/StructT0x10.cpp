#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Container.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructT0x10.hpp"


void StructT0x10::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::T0x10, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    const uint16_t sth = ds.readUint16();

    spdlog::trace("sth = {}", sth);

    const int16_t pointX = ds.readInt16();
    const int16_t pointY = ds.readInt16();

    spdlog::trace("pointX = {}", pointX);
    spdlog::trace("pointY = {}", pointY);

    const uint32_t maybeId = ds.readUint32();

    spdlog::trace("maybeId = {}", maybeId);

    const uint32_t unkownInt = ds.readUint32();

    spdlog::trace("unkownInt = {}", unkownInt);

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}