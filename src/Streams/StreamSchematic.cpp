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

    // Structure structure = read_prefixes(2);
    Structure structure = auto_read_prefixes();

    if(structure != Structure::SchLib)
    {
        // @todo throw some exception
    }
    // mDs.printUnknownData(12, std::string(__func__) + " - 0");

    readPreamble();

    std::string schematic_name = mDs.readStringLenZeroTerm();

    mDs.printUnknownData(4, std::string(__func__) + " - 1");

    const uint16_t schematicPages = mDs.readUint16();

    for(size_t i = 0u; i < schematicPages; ++i)
    {
        std::string page_name = mDs.readStringLenZeroTerm();
        spdlog::debug(page_name);
    }

    const uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        mDs.printUnknownData(4, std::string(__func__) + " - 1");
    }

    const uint16_t len2 = mDs.readUint16();

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