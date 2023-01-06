#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructAlias.hpp"


void StructAlias::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    locX = mDs.get().readInt32();
    locY = mDs.get().readInt32();

    spdlog::debug("locX = {}", locX);
    spdlog::debug("locY = {}", locY);

    color = ToColor(mDs.get().readUint32());

    spdlog::debug("color = {}", ::to_string(color));

    rotation = ToRotation(mDs.get().readUint32()); // @todo Why is it 4 byte? Probably increase Rotation size

    spdlog::debug("rotation = {}", ::to_string(rotation));

    uint32_t textFontIdx = mDs.get().readUint32();

    spdlog::debug("Alias fontIdx = {}", textFontIdx);

    name = mDs.get().readStringLenZeroTerm();

    spdlog::debug("name = {}", name);

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}