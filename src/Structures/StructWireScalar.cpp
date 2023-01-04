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

    auto_read_prefixes();

    readPreamble();

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

    mDs.printUnknownData(1, std::string(__func__) + " - 1");

    spdlog::debug("mByteOffset = {}", mByteOffset);

    if(mByteOffset == 0x3d)
    {
        mDs.printUnknownData(2, std::string(__func__) + " - 2");
    }
    else if(mByteOffset > 0x3d)
    {
        const uint16_t len = mDs.readUint16();

        spdlog::debug("len = {}", len);

        for(size_t i = 0u; i < len; ++i)
        {
            // @todo len should always be 1 and the read structure should be 'Alias'
            readStructure(); // @todo push
        }
    }

    mDs.printUnknownData(2, std::string(__func__) + " - 3");

    obj.wireLineWidth = ToLineWidth(mDs.readUint32());
    obj.wireLineStyle = ToLineStyle(mDs.readUint32());

    spdlog::debug("wireLineWidth = {} | wireLineStyle = {}",
        to_string(obj.wireLineWidth), to_string(obj.wireLineStyle));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}