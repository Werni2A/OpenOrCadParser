#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructGraphicBoxInst.hpp"


// @todo is this a specialized instance for Rects or general for all types?
void StructGraphicBoxInst::read(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion();
    }

    FutureDataLst localFutureLst{mDs};

    auto_read_prefixes(Structure::GraphicBoxInst, localFutureLst);

    readPreamble();

    mDs.get().printUnknownData(11, getMethodName(this, __func__) + ": 0");

    dbId = mDs.get().readUint32();

    spdlog::trace("dbId = {}", dbId);

    locY = mDs.get().readInt16();
    locX = mDs.get().readInt16();

    spdlog::trace("locX = {}", locX);
    spdlog::trace("locY = {}", locY);

    y2 = mDs.get().readInt16();
    x2 = mDs.get().readInt16();

    spdlog::trace("y2 = {}", y2);
    spdlog::trace("x2 = {}", x2);

    x1 = mDs.get().readInt16();
    y1 = mDs.get().readInt16();

    spdlog::trace("x1 = {}", x1);
    spdlog::trace("y1 = {}", y1);

    color = ToColor(mDs.get().readUint16()); // @todo is it really not a 4 byte value?

    spdlog::trace("color = {}", ::to_string(color));

    mDs.get().printUnknownData(5, getMethodName(this, __func__) + ": 1");

    sthInPages0 = dynamic_pointer_cast<StructSthInPages0>(readStructure());

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}