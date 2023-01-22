#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructGraphicBoxInst.hpp"


// @todo is this a specialized instance for Rects or general for all types?
void StructGraphicBoxInst::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    mDs.get().printUnknownData(11, std::string(__func__) + " - 0");

    dbId = mDs.get().readUint32();

    locY = mDs.get().readInt16();
    locX = mDs.get().readInt16();

    y2 = mDs.get().readInt16();
    x2 = mDs.get().readInt16();

    x1 = mDs.get().readInt16();
    y1 = mDs.get().readInt16();

    color = ToColor(mDs.get().readUint16()); // @todo is it really not a 4 byte value?

    mDs.get().printUnknownData(5, std::string(__func__) + " - 1");

    // @todo Only Rect as a shape would make sense here. Maybe this should be passed
    //       as a parameter to readSthInPages0 to check this condition. Further,
    //       parseStructure should always call readSthInPages0.
    readStructure();

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}