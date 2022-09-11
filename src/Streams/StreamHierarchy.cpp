#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Parser.hpp"


// @todo return real data object
bool Parser::readStreamHierarchy()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    bool obj = false;

    mDs.printUnknownData(9, std::string(__func__) + " - 0");

    std::string schematicName = mDs.readStringLenZeroTerm();

    mDs.printUnknownData(9, std::string(__func__) + " - 1");

    const uint16_t netLen = mDs.readUint16();

    for(size_t i = 0u; i < netLen; ++i)
    {
        Structure structure = auto_read_prefixes();

        readPreamble();

        readStructure(structure);
    }

    const uint16_t len1 = mDs.readUint16();

    spdlog::debug("len1 = {}", len1);

    for(size_t i = 0u; i < len1; ++i)
    {
        Structure structure = auto_read_prefixes();

        readPreamble();

        readStructure(structure);
    }

    readPreamble();

    const uint16_t len2 = mDs.readUint16();

    spdlog::debug("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        Structure structure = auto_read_prefixes();

        readPreamble();

        readStructure(structure);

        const uint16_t len3 = mDs.readUint16();

        spdlog::debug("len3 = {}", len3);

        for(size_t i = 0u; i < len3; ++i)
        {
            Structure structure = auto_read_prefixes();

            readPreamble();

            readStructure(structure);
        }
    }

    mDs.printUnknownData(10, std::string(__func__) + " - 3");

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return obj;
}