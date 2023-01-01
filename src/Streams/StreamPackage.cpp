#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Parser.hpp"
#include "Streams/StreamPackage.hpp"


[[maybe_unused]]
static FileFormatVersion predictVersion(DataStream& aDs, Parser& aParser)
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
            aParser.readStreamPackage(version);
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


StreamPackage Parser::readStreamPackage(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    StreamPackage obj;

    Structure structure;

    const uint16_t sectionCount = mDs.readUint16();

    spdlog::info("sectionCount = {}", sectionCount);

    for(size_t i = 0u; i < sectionCount; ++i)
    {
        Structure structure = auto_read_prefixes();
        obj.structures.push_back(readStructure(structure));
    }

    // @todo maybe number of views (Convert, Normal) or number of units in the current view
    const uint16_t len2 = mDs.readUint16();

    spdlog::info("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        Structure structure = auto_read_prefixes();
        obj.structures.push_back(readStructure(structure));
    }

    // @todo Probably only StructSymbolPinScalar
    const uint16_t len3 = mDs.readUint16();

    spdlog::info("len3 = {}", len3);

    for(size_t i = 0u; i < len3; ++i)
    {
        Structure structure = auto_read_prefixes();
        obj.structures.push_back(readStructure(structure));

        const uint8_t early_out = mDs.peek(1)[0];
        spdlog::critical("early_out = {}", early_out);

        if(early_out == 0U)
        {
            // @todo does not always occur, even in the same file. Maybe its some byte alignment?
            mDs.printUnknownData(1, "Early Out Indicator");
            break;
        }
    }

    const uint16_t len4 = mDs.readUint16();

    spdlog::info("len4 = {}", len4);

    for(size_t i = 0u; i < len4; ++i)
    {
        Structure structure = auto_read_prefixes();
        obj.structures.push_back(readStructure(structure));
    }

    {
        Structure structure = auto_read_prefixes();
        obj.structures.push_back(readStructure(structure));
    }

    // I guess its always a PinIdxMapping (or multiple)
    // @todo should be the unit of the package
    const uint16_t len5 = mDs.readUint16();

    spdlog::info("len5 = {}", len5);

    for(size_t i = 0u; i < len5; ++i)
    {
        Structure structure = auto_read_prefixes();
        obj.structures.push_back(readStructure(structure));
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::debug(to_string(obj));

    return obj;
}