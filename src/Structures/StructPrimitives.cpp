#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructPrimitives.hpp"


FileFormatVersion StructPrimitives::predictVersion()
{
    FileFormatVersion prediction = FileFormatVersion::Unknown;

    const std::vector<FileFormatVersion> versions{
        FileFormatVersion::A,
        FileFormatVersion::B,
        FileFormatVersion::C
    };

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

    return prediction;
}


void StructPrimitives::read(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    name = mDs.get().readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    mDs.get().assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably a string

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    // -------------------------------------------------------------------

    readOptionalTrailingFuture();

    // const uint16_t geometryCount = mDs.get().readUint16();
    // spdlog::trace("geometryCount = {}", geometryCount);

    // for(size_t i = 0u; i < geometryCount; ++i)
    // {
    //     spdlog::trace("i of geometryCount = {}", i);

    //     if(i > 0u)
    //     {
    //         if(gFileFormatVersion == FileFormatVersion::B)
    //         {
    //             // Structure structure = read_prefixes(3);
    //             Structure structure = auto_read_prefixes();
    //         }

    //         if(gFileFormatVersion >= FileFormatVersion::B)
    //         {
    //             readPreamble();
    //         }
    //     }

    //     const Primitive primitive = readPrefixPrimitive();

    //     readPrimitive(primitive);

    //     // if(gFileFormatVersion == FileFormatVersion::A)
    //     // {
    //     //     mDs.get().printUnknownData(8, std::string(__func__) + " - 3.5");
    //     // }
    // }

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}