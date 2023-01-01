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

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    StructAlias obj;

    obj.locX = mDs.readInt32();
    obj.locY = mDs.readInt32();

    spdlog::debug("locX = {}", obj.locX);
    spdlog::debug("locY = {}", obj.locY);

    obj.color = ToColor(mDs.readUint32());

    spdlog::debug("color = {}", to_string(obj.color));

    obj.rotation = ToRotation(mDs.readUint32()); // @todo Why is it 4 byte? Probably increase Rotation size

    spdlog::debug("rotation = {}", to_string(obj.rotation));

    uint32_t textFontIdx = mDs.readUint32();

    spdlog::debug("Alias fontIdx = {}", textFontIdx);

    obj.name = mDs.readStringLenZeroTerm();

    spdlog::debug("name = {}", obj.name);

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}