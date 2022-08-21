#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../General.hpp"

#include "Package.hpp"

#include "../Parser.hpp"


Package Parser::readPackage()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    Package obj;

    Structure structure;

    const uint16_t sectionCount = mDs.readUint16();

    for(size_t i = 0u; i < sectionCount; ++i)
    {
        spdlog::debug("Marker 0");

        structure = read_type_prefix_long();
        readConditionalPreamble(structure);
        pushStructure(parseStructure(structure), obj);

        spdlog::debug("Marker 1");

        structure = read_type_prefix();
        readConditionalPreamble(structure);
        spdlog::debug("Marker 1.5");
        pushStructure(parseStructure(structure), obj);

        // if(structure == Structure::GeoDefinition && mFileFormatVersion == FileFormatVersion::C)
        //     mDs.discardBytes(6);

        // structure = read_type_prefix();

        spdlog::debug("Marker 2");

        if(mFileFormatVersion == FileFormatVersion::B)
        {
            mDs.assumeData({0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");
            // mDs.printUnknownData(8, std::string(__func__) + " - 1");
        }
        else if(mFileFormatVersion >= FileFormatVersion::C)
        {
            readPreamble();
        }

        spdlog::debug("Marker 3");

        mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 2");
        mDs.printUnknownData(4, std::string(__func__) + " - 2");
        // mDs.printUnknownData(12, std::string(__func__) + " - 2");

        const uint16_t followingLen1 = mDs.readUint16();

        for(size_t i = 0u; i < followingLen1; ++i)
        {
            spdlog::debug("0x{:08x}: followingLen1 Iteration {}/{}",
                mDs.getCurrentOffset(), i + 1, followingLen1);

            structure = read_type_prefix();
            readConditionalPreamble(structure);
            pushStructure(parseStructure(structure), obj);
        }

        spdlog::debug("Marker 4");

        const uint16_t followingLen2 = mDs.readUint16();

        for(size_t i = 0u; i < followingLen2; ++i)
        {
            spdlog::debug("0x{:08x}: followingLen2 Iteration {}/{}",
                mDs.getCurrentOffset(), i + 1, followingLen2);

            structure = read_type_prefix();
            readConditionalPreamble(structure);
            pushStructure(parseStructure(structure), obj);
        }

        spdlog::debug("Marker 5");

        /*
        mDs.printUnknownData(22, std::string(__func__) + " - w");
        structure = read_type_prefix();
        readConditionalPreamble(structure);
        // readPreamble();
        */

        obj.generalProperties = readGeneralProperties();

        spdlog::debug("Section count {} finished", i);
    }

    spdlog::debug("Marker 6");

    // @todo how often does it repeat? This should be specified somewhere....
    for(size_t i = 0u; true; ++i)
    {
        if(mDs.isEoF())
        {
            spdlog::debug("i = {}", i);
            break;
        }

        spdlog::debug("Marker 7");

        if(i == 0u)
        {
            structure = read_type_prefix_long();
        }
        else
        {
            structure = read_type_prefix();
        }

        readConditionalPreamble(structure);
        pushStructure(parseStructure(structure), obj);
    }

    spdlog::debug("Marker 8");

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::debug(to_string(obj));

    return obj;
}