#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructWireScalar.hpp"


void StructWireScalar::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    dbId = mDs.get().readUint32();

    spdlog::debug("dbId = {}", dbId);

    mDs.get().printUnknownData(4, std::string(__func__) + " - 0");

    wireColor = ToColor(mDs.get().readUint32());

    spdlog::debug("wireColor = {}", ::to_string(wireColor));

    startX = mDs.get().readInt32();
    startY = mDs.get().readInt32();
    endX   = mDs.get().readInt32();
    endY   = mDs.get().readInt32();

    spdlog::debug("startX = {} | startY = {} | endX = {} | endY = {}",
        startX, startY, endX, endY);

    mDs.get().printUnknownData(1, std::string(__func__) + " - 1");

    // @todo mByteOffset is uninitialized....!
    spdlog::debug("mByteOffset = {}", mByteOffset);

    if(mByteOffset == 0x3d)
    {
        mDs.get().printUnknownData(2, std::string(__func__) + " - 2");
    }
    else if(mByteOffset > 0x3d)
    {
        const uint16_t len = mDs.get().readUint16();

        spdlog::debug("len = {}", len);

        for(size_t i = 0u; i < len; ++i)
        {
            // @todo len should always be 1 and the read structure should be 'Alias'
            readStructure(); // @todo push
        }
    }

    mDs.get().printUnknownData(2, std::string(__func__) + " - 3");

    wireLineWidth = ToLineWidth(mDs.get().readUint32());
    wireLineStyle = ToLineStyle(mDs.get().readUint32());

    spdlog::debug("wireLineWidth = {} | wireLineStyle = {}",
        ::to_string(wireLineWidth), ::to_string(wireLineStyle));

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}