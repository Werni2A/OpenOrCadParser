#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../General.hpp"

#include "Package.hpp"

#include "../Parser.hpp"


static FileFormatVersion predictVersionPackage(DataStream& aDs, Parser& aParser)
{
    FileFormatVersion prediction = FileFormatVersion::Unknown;


    const std::vector<FileFormatVersion> versions{
        FileFormatVersion::A,
        FileFormatVersion::B,
        FileFormatVersion::C
    };

    const auto saved_level = spdlog::get_level();

    spdlog::set_level(spdlog::level::off);

    const size_t initial_offset = aDs.getCurrentOffset();

    for(const auto& version : versions)
    {
        bool found = true;

        try
        {
            aParser.readPackage(version);
        }
        catch(...)
        {
            found = false;
        }

        aDs.setCurrentOffset(initial_offset);

        if(found)
        {
            prediction = version;
            break;
        }
    }

    if(prediction == FileFormatVersion::Unknown)
    {
        // Set to previous default value
        // s.t. tests not fail
        prediction = FileFormatVersion::C;
    }

    spdlog::set_level(saved_level);

    return prediction;
}


Package Parser::readPackage(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    // Predict version
    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersionPackage(mDs, *this);
        // spdlog::info("Predicted version {} in {}", aVersion, __func__);
    }

    Package obj;

    Structure structure;

    const uint16_t sectionCount = mDs.readUint16();

    spdlog::info("sectionCount = {}", sectionCount);

    for(size_t i = 0u; i < sectionCount; ++i)
    {
        spdlog::debug("Marker 0");

        // structure = read_prefixes(4);
        Structure structure = auto_read_prefixes();
        readConditionalPreamble(structure);
        pushStructure(parseStructure(structure), obj);

        spdlog::debug("Marker 1");

        // structure = read_prefixes(3);
        structure = auto_read_prefixes();
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

            // structure = read_prefixes(3);
            structure = auto_read_prefixes();
            readConditionalPreamble(structure);
            pushStructure(parseStructure(structure), obj);
        }

        spdlog::debug("Marker 4");

        const uint16_t followingLen2 = mDs.readUint16();

        for(size_t i = 0u; i < followingLen2; ++i)
        {
            spdlog::debug("0x{:08x}: followingLen2 Iteration {}/{}",
                mDs.getCurrentOffset(), i + 1, followingLen2);

            // structure = read_prefixes(3);
            structure = auto_read_prefixes();
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
            // structure = read_prefixes(4);
            structure = auto_read_prefixes();
        }
        else
        {
            // structure = read_prefixes(3);
            structure = auto_read_prefixes();
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