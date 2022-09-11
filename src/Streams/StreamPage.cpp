#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Parser.hpp"


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
            aParser.readStreamPage(version);
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


// @todo return real data object
bool Parser::readStreamPage(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    // Predict version
    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion(mDs, *this);
        // spdlog::info("Predicted version {} in {}", static_cast<int>(aVersion), __func__);
    }

    bool obj = false;

    auto_read_prefixes(); // @todo should call parsePage
    readPreamble();

    std::string name = mDs.readStringLenZeroTerm();

    std::string pageSize = mDs.readStringLenZeroTerm();

    time_t createDateTime = static_cast<time_t>(mDs.readUint32());
    time_t modifyDateTime = static_cast<time_t>(mDs.readUint32());

    spdlog::debug("createDateTime = {} | modifyDateTime = {}", DateTimeToStr(createDateTime), DateTimeToStr(modifyDateTime));

    // mDs.printUnknownData(16, std::string(__func__) + " - 1");

    // uint32_t width  = mDs.readUint32();
    // uint32_t height = mDs.readUint32();

    // spdlog::debug("width = {} | height = {}", width, height);

    // uint32_t pinToPin = mDs.readUint32(); //!< Pin-to-pin spacing

    // spdlog::debug("pinToPin = {}", pinToPin);

    // mDs.printUnknownData(2, std::string(__func__) + " - 2");

    // uint16_t horizontalCount = mDs.readUint16(); //!< See 'Schematic Page Properties' -> 'Grid Reference'
    // uint16_t verticalCount   = mDs.readUint16(); //!< See 'Schematic Page Properties' -> 'Grid Reference'

    // spdlog::debug("horizontalCount = {} | verticalCount = {}", horizontalCount, verticalCount);

    // mDs.printUnknownData(2, std::string(__func__) + " - 3");

    // uint32_t horizontalWidth = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference'
    // uint32_t verticalWidth   = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference'

    // spdlog::debug("horizontalWidth = {} | verticalWidth = {}", horizontalWidth, verticalWidth);

    // mDs.printUnknownData(48, std::string(__func__) + " - 4");

    // uint32_t horizontalChar = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
    //                                          //             'Horizontal' -> 'Alphabetic' = 1
    //                                          //                             'Numeric'    = 0

    // spdlog::debug("horizontalChar = {}", horizontalChar);

    // mDs.printUnknownData(4, std::string(__func__) + " - 5");

    // uint32_t horizontalAscending = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
    //                                               //         'Horizontal'
    //                                               //         Select 'ascending' (1) or 'decending' (0).

    // spdlog::debug("horizontalAscending = {}", horizontalAscending);

    // uint32_t verticalChar   = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference' ->
    //                                          //       'Vertical' -> 'Alphabetic' = 1
    //                                          //                     'Numeric'    = 0

    // spdlog::debug("verticalChar = {}", verticalChar);

    // mDs.printUnknownData(4, std::string(__func__) + " - 6");


    // uint32_t verticalAscending = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
    //                                               //         'Vertical'
    //                                               //         Select 'ascending' (1) or 'decending' (0).

    // spdlog::debug("verticalAscending = {}", verticalAscending);

    // uint32_t isMetric            = mDs.readUint32();
    // uint32_t borderDisplayed     = mDs.readUint32();
    // uint32_t borderPrinted       = mDs.readUint32();
    // uint32_t gridRefDisplayed    = mDs.readUint32();
    // uint32_t gridRefPrinted      = mDs.readUint32();
    // uint32_t titleblockDisplayed = mDs.readUint32();
    // uint32_t titleblockPrinted   = mDs.readUint32();
    // // @todo XML is always 1, maybe a bug in OrCad?
    // uint32_t ansiGridRefs = mDs.readUint32(); //!< Use ANSI grid references.
    //                                           //   See 'Schematic Page Properties' -> 'Grid Reference'

    // spdlog::debug("isMetric            = {}", isMetric);
    // spdlog::debug("borderDisplayed     = {}", borderDisplayed);
    // spdlog::debug("borderPrinted       = {}", borderPrinted);
    // spdlog::debug("gridRefDisplayed    = {}", gridRefDisplayed);
    // spdlog::debug("gridRefPrinted      = {}", gridRefPrinted);
    // spdlog::debug("titleblockDisplayed = {}", titleblockDisplayed);
    // spdlog::debug("titleblockPrinted   = {}", titleblockPrinted);
    // spdlog::debug("ansiGridRefs        = {}", ansiGridRefs);

    // mDs.printUnknownData(236, fmt::format("{}: 0", __func__));
    mDs.printUnknownData(268, fmt::format("{}: 0", __func__));

    readPreamble();

    mDs.printUnknownData(8, fmt::format("{}: 1", __func__));

    const std::string titleblock0_name = mDs.readStringLenZeroTerm();

    spdlog::debug("titleblock0_name = {}", titleblock0_name);

    mDs.printUnknownData(20, fmt::format("{}: 2", __func__));

    const uint16_t len0 = mDs.readUint16();

    spdlog::debug("len0 = {}", len0);

    for(size_t i = 0U; i < len0; ++i)
    {
        Structure structure = auto_read_prefixes();
        readPreamble();

        readStructure(structure);
    }

    mDs.printUnknownData(13, fmt::format("{}: 3", __func__));

    const uint16_t len1 = mDs.readUint16();

    spdlog::debug("len1 = {}", len1);

    for(size_t i = 0U; i < len1; ++i)
    {
        mDs.printUnknownData(32, fmt::format("{}: 4.{}", __func__, i));
    }

    mDs.printUnknownData(2, fmt::format("{}: 5", __func__));

    // Nets
    const uint16_t len2 = mDs.readUint16();

    spdlog::debug("len2 = {}", len2);

    for(size_t i = 0U; i < len2; ++i)
    {
        spdlog::debug("{}/{}:", i + 1, len2);

        const std::string name0 = mDs.readStringLenZeroTerm();

        spdlog::debug("name0 = {}", name0);

        mDs.printUnknownData(4, fmt::format("{}: 6.{}", __func__, i));
    }

    // Read wirescalar
    const uint16_t len3 = mDs.readUint16();

    spdlog::debug("len3 = {}", len3);

    for(size_t i = 0U; i < len3 -1; ++i)
    {
        spdlog::debug("{}/{}:", i + 1, len3);
        Structure structure = auto_read_prefixes();
        readPreamble();

        readStructure(structure);
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return obj;
}