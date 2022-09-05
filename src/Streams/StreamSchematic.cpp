#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Parser.hpp"


// @todo return real data object
bool Parser::readStreamSchematic()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    bool obj = false;

    Structure structure = auto_read_prefixes();

    readPreamble();

    // @todo extract into parseSchematic method that is called with structure

    const std::string schematic_name = mDs.readStringLenZeroTerm();

    spdlog::debug("schematic_name = {}", schematic_name);

    mDs.printUnknownData(4, std::string(__func__) + " - 1");

    const uint16_t schematicPages = mDs.readUint16();

    spdlog::debug("schematicPages = {}", schematicPages);

    for(size_t i = 0u; i < schematicPages; ++i)
    {
        const std::string page_name = mDs.readStringLenZeroTerm();
        spdlog::debug("page_name = {}", page_name);
    }

    const uint16_t len = mDs.readUint16();

    spdlog::debug("len = {}", len);

    for(size_t i = 0u; i < len; ++i)
    {
        mDs.printUnknownData(4, std::string(__func__) + " - 1");
    }

    const uint16_t len2 = mDs.readUint16();

    spdlog::debug("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        mDs.printUnknownData(5, std::string(__func__) + " - 2");
    }

    mDs.printUnknownData(4, std::string(__func__) + " - 3");

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return obj;
}