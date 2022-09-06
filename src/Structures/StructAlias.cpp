#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructAlias.hpp"


StructAlias Parser::readStructAlias()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructAlias obj;

    int32_t locX = mDs.readInt32();
    int32_t locY = mDs.readInt32();

    spdlog::debug("locX = {} | locY = {}", locX, locY);

    Color color = ToColor(mDs.readUint32());

    spdlog::debug("color = {}", to_string(color));

    Rotation rotation = ToRotation(mDs.readUint32()); // @todo Why is it 4 byte? Probably increase Rotation size

    spdlog::debug("rotation = {}", to_string(rotation));

    uint16_t textFontIdx = mDs.readUint16(); // @todo educated guess

    spdlog::debug("Alias fontIdx = {}", textFontIdx);

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    std::string name = mDs.readStringLenZeroTerm();

    spdlog::debug("name = {}", name);

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}