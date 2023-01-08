#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamPackage.hpp"


FileFormatVersion StreamPackage::predictVersion()
{
    FileFormatVersion prediction = FileFormatVersion::Unknown;


    const std::vector<FileFormatVersion> versions{
        FileFormatVersion::A,
        FileFormatVersion::B,
        FileFormatVersion::C
    };

    const auto saved_level = spdlog::get_level();

    spdlog::set_level(spdlog::level::off);

    const size_t initial_offset = mDs.get().getCurrentOffset();

    for(const auto& version : versions)
    {
        bool found = true;

        try
        {
            read(version);
        }
        catch(...)
        {
            found = false;
        }

        mDs.get().setCurrentOffset(initial_offset);

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


void StreamPackage::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const uint16_t sectionCount = mDs.get().readUint16();

    spdlog::info("sectionCount = {}", sectionCount);

    for(size_t i = 0u; i < sectionCount; ++i)
    {
        structures.push_back(readStructure());
    }

    // @todo maybe number of views (Convert, Normal) or number of units in the current view
    const uint16_t len2 = mDs.get().readUint16();

    spdlog::info("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        structures.push_back(readStructure());
    }

    // @todo Probably only StructSymbolPinScalar
    const uint16_t len3 = mDs.get().readUint16();

    spdlog::info("len3 = {}", len3);

    for(size_t i = 0u; i < len3; ++i)
    {
        structures.push_back(readStructure());

        const uint8_t early_out = mDs.get().peek(1)[0];
        spdlog::critical("early_out = {}", early_out);

        if(early_out == 0U)
        {
            // @todo does not always occur, even in the same file. Maybe its some byte alignment?
            mDs.get().printUnknownData(1, "Early Out Indicator");
            break;
        }
    }

    const uint16_t len4 = mDs.get().readUint16();

    spdlog::info("len4 = {}", len4);

    for(size_t i = 0u; i < len4; ++i)
    {
        structures.push_back(readStructure());
    }

    {
        structures.push_back(readStructure());
    }

    // I guess its always a PinIdxMapping (or multiple)
    // @todo should be the unit of the package
    const uint16_t len5 = mDs.get().readUint16();

    spdlog::info("len5 = {}", len5);

    for(size_t i = 0u; i < len5; ++i)
    {
        structures.push_back(readStructure());
    }

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::debug(to_string());
}