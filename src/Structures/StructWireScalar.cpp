#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructWireScalar.hpp"


StructWireScalar Parser::readStructWireScalar()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructWireScalar obj;

    obj.dbId = mDs.readUint32();

    spdlog::debug("dbId = {}", obj.dbId);

    mDs.printUnknownData(4, std::string(__func__) + " - 0");

    obj.wireColor = ToColor(mDs.readUint32());

    spdlog::debug("wireColor = {}", to_string(obj.wireColor));

    obj.startX = mDs.readInt32();
    obj.startY = mDs.readInt32();
    obj.endX   = mDs.readInt32();
    obj.endY   = mDs.readInt32();

    spdlog::debug("startX = {} | startY = {} | endX = {} | endY = {}",
        obj.startX, obj.startY, obj.endX, obj.endY);

    if(thisFuture.has_value())
    {
        mDs.printUnknownData(thisFuture.value().getStopOffset() - mDs.getCurrentOffset(), "Some Data...");
    }
    else
    {
        mDs.printUnknownData(3, std::string(__func__) + " - 1");
    }

    // obj.wireLineWidth = ToLineWidth(mDs.readUint32());
    // obj.wireLineStyle = ToLineStyle(mDs.readUint32());

    // spdlog::debug("wireLineWidth = {} | wireLineStyle = {}",
    //     to_string(obj.wireLineWidth), to_string(obj.wireLineStyle));

    Structure structure = auto_read_prefixes();
    readPreamble();

    readStructure(structure);

    mDs.printUnknownData(10, std::string(__func__) + " - 2");

    sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}