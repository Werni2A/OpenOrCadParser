#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructT0x34.hpp"


void StructT0x34::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    // FutureDataLst localFutureLst{mDs};

    // auto_read_prefixes(Structure::T0x34, localFutureLst);

    // readPreamble();

    // @todo Looks like Primitive types!
    //     0 Byte = structure id
    // 1 - 4 Byte = struct len
    // 5 - 8 Byte = zero vector
    mDs.get().printUnknownData(9, getMethodName(this, __func__) + ": 0");

    id = mDs.get().readUint32();

    spdlog::trace("id = {}", id);

    const std::string unknownStr = mDs.get().readStringLenZeroTerm();

    spdlog::trace("unknownStr = {}", unknownStr);

    const uint32_t unknownInt = mDs.get().readUint32();

    spdlog::trace("unknownInt = {}", unknownInt);

    color = ToColor(mDs.get().readUint32());

    spdlog::trace("color = {}", ::to_string(color));

    lineStyle = ToLineStyle(mDs.get().readUint32());

    spdlog::trace("lineStyle = {}", ::to_string(lineStyle));

    lineWidth = ToLineWidth(mDs.get().readUint32());

    spdlog::trace("lineWidth = {}", ::to_string(lineWidth));

    // localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}