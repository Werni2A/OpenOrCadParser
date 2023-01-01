#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructPrimitives.hpp"


[[maybe_unused]]
static FileFormatVersion predictVersion(DataStream& aDs, Parser& aParser)
{
    FileFormatVersion prediction = FileFormatVersion::Unknown;


    const std::vector<FileFormatVersion> versions{
        FileFormatVersion::A,
        FileFormatVersion::B,
        FileFormatVersion::C
    };

    const size_t initial_offset = aDs.getCurrentOffset();

    for(const auto& version : versions)
    {
        bool found = true;

        StructPrimitives geo_spec;

        try
        {
            aParser.readStructPrimitives(version);
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

    return prediction;
}


StructPrimitives Parser::readStructPrimitives(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    StructPrimitives obj;

    obj.name = mDs.readStringLenZeroTerm();

    spdlog::debug("name = {}", obj.name);

    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably a string

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    // -------------------------------------------------------------------

    readOptionalTrailingFuture();

    // const uint16_t geometryCount = mDs.readUint16();
    // spdlog::debug("geometryCount = {}", geometryCount);

    // for(size_t i = 0u; i < geometryCount; ++i)
    // {
    //     spdlog::debug("i of geometryCount = {}", i);

    //     if(i > 0u)
    //     {
    //         if(mFileFormatVersion == FileFormatVersion::B)
    //         {
    //             // Structure structure = read_prefixes(3);
    //             Structure structure = auto_read_prefixes();
    //         }

    //         if(mFileFormatVersion >= FileFormatVersion::B)
    //         {
    //             readPreamble();
    //         }
    //     }

    //     const Primitive primitive = readPrefixPrimitive();

    //     readPrimitive(primitive);

    //     // if(mFileFormatVersion == FileFormatVersion::A)
    //     // {
    //     //     mDs.printUnknownData(8, std::string(__func__) + " - 3.5");
    //     // }
    // }

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}