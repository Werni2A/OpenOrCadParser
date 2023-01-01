#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructGraphicBoxInst.hpp"


// @todo is this a specialized instance for Rects or general for all types?
StructGraphicBoxInst Parser::readStructGraphicBoxInst()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    StructGraphicBoxInst obj;

    mDs.printUnknownData(11, std::string(__func__) + " - 0");

    obj.dbId = mDs.readUint32();

    obj.locY = mDs.readInt16();
    obj.locX = mDs.readInt16();

    obj.y2 = mDs.readInt16();
    obj.x2 = mDs.readInt16();

    obj.x1 = mDs.readInt16();
    obj.y1 = mDs.readInt16();

    obj.color = ToColor(mDs.readUint16()); // @todo is it really not a 4 byte value?

    mDs.printUnknownData(5, std::string(__func__) + " - 1");

    // @todo Only Rect as a shape would make sense here. Maybe this should be passed
    //       as a parameter to readSthInPages0 to check this condition. Further,
    //       parseStructure should always call readSthInPages0.
    // Structure structure = read_prefixes(4);
    Structure structure = auto_read_prefixes();
    readStructure(structure);

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}