#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../General.hpp"

#include "../Parser.hpp"


// @todo return real data object
bool Parser::readPage()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    bool obj = false;

    // readDevHelper();
    // return;

    mDs.printUnknownData(21, std::string(__func__) + " - 0");
    readPreamble();

    std::string name = mDs.readStringLenZeroTerm();

    std::string pageSize = mDs.readStringLenZeroTerm();

    time_t createDateTime = static_cast<time_t>(mDs.readUint32());
    time_t modifyDateTime = static_cast<time_t>(mDs.readUint32());

    spdlog::debug("createDateTime = {} | modifyDateTime = {}", DateTimeToStr(createDateTime), DateTimeToStr(modifyDateTime));

    mDs.printUnknownData(16, std::string(__func__) + " - 1");

    uint32_t width  = mDs.readUint32();
    uint32_t height = mDs.readUint32();

    spdlog::debug("width = {} | height = {}", width, height);

    uint32_t pinToPin = mDs.readUint32(); //!< Pin-to-pin spacing

    spdlog::debug("pinToPin = {}", pinToPin);

    mDs.printUnknownData(2, std::string(__func__) + " - 2");

    uint16_t horizontalCount = mDs.readUint16(); //!< See 'Schematic Page Properties' -> 'Grid Reference'
    uint16_t verticalCount   = mDs.readUint16(); //!< See 'Schematic Page Properties' -> 'Grid Reference'

    spdlog::debug("horizontalCount = {} | verticalCount = {}", horizontalCount, verticalCount);

    mDs.printUnknownData(2, std::string(__func__) + " - 3");

    uint32_t horizontalWidth = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference'
    uint32_t verticalWidth   = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference'

    spdlog::debug("horizontalWidth = {} | verticalWidth = {}", horizontalWidth, verticalWidth);

    mDs.printUnknownData(48, std::string(__func__) + " - 4");

    uint32_t horizontalChar = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
                                             //             'Horizontal' -> 'Alphabetic' = 1
                                             //                             'Numeric'    = 0

    spdlog::debug("horizontalChar = {}", horizontalChar);

    mDs.printUnknownData(4, std::string(__func__) + " - 5");

    uint32_t horizontalAscending = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
                                                  //         'Horizontal'
                                                  //         Select 'ascending' (1) or 'decending' (0).

    spdlog::debug("horizontalAscending = {}", horizontalAscending);

    uint32_t verticalChar   = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference' ->
                                             //       'Vertical' -> 'Alphabetic' = 1
                                             //                     'Numeric'    = 0

    spdlog::debug("verticalChar = {}", verticalChar);

    mDs.printUnknownData(4, std::string(__func__) + " - 6");


    uint32_t verticalAscending = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
                                                  //         'Vertical'
                                                  //         Select 'ascending' (1) or 'decending' (0).

    spdlog::debug("verticalAscending = {}", verticalAscending);

    uint32_t isMetric            = mDs.readUint32();
    uint32_t borderDisplayed     = mDs.readUint32();
    uint32_t borderPrinted       = mDs.readUint32();
    uint32_t gridRefDisplayed    = mDs.readUint32();
    uint32_t gridRefPrinted      = mDs.readUint32();
    uint32_t titleblockDisplayed = mDs.readUint32();
    uint32_t titleblockPrinted   = mDs.readUint32();
    // @todo XML is always 1, maybe a bug in OrCad?
    uint32_t ansiGridRefs = mDs.readUint32(); //!< Use ANSI grid references.
                                              //   See 'Schematic Page Properties' -> 'Grid Reference'

    spdlog::debug("isMetric            = {}", isMetric);
    spdlog::debug("borderDisplayed     = {}", borderDisplayed);
    spdlog::debug("borderPrinted       = {}", borderPrinted);
    spdlog::debug("gridRefDisplayed    = {}", gridRefDisplayed);
    spdlog::debug("gridRefPrinted      = {}", gridRefPrinted);
    spdlog::debug("titleblockDisplayed = {}", titleblockDisplayed);
    spdlog::debug("titleblockPrinted   = {}", titleblockPrinted);
    spdlog::debug("ansiGridRefs        = {}", ansiGridRefs);

    const uint16_t lenA = mDs.readUint16();

    spdlog::debug("lenA = {}", lenA);

    for(size_t i = 0u; i < lenA; ++i)
    {
        mDs.printUnknownData(8, std::string(__func__) + " - a");
    }

    const uint16_t len0 = mDs.readUint16();

    spdlog::debug("len0 = {}", len0);

    for(size_t i = 0u; i < len0; ++i)
    {
        mDs.printUnknownData(32, std::string(__func__) + " - 8");
    }

    // mDs.printUnknownData(30, std::string(__func__) + " - 7");


    // mDs.printUnknownData(38, std::string(__func__) + " - 1.5");

    // @todo required for CONTENT page but not for the others? This offset must be somehow
    //       dynamic
    // mDs.printUnknownData(14, std::string(__func__) + " - 1.6");

    mDs.printUnknownData(2, std::string(__func__) + " - 9");

    const uint16_t len1 = mDs.readUint16();

    spdlog::debug("len1 = {}", len1);

    for(size_t i = 0u; i < len1; ++i)
    {
        std::string name = mDs.readStringLenZeroTerm();
        mDs.printUnknownData(4, std::string(__func__) + " - 10");
    }

    const uint16_t len2 = mDs.readUint16();

    spdlog::debug("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        // if(i > 0u)
        // {
        //     readPreamble();
        //     mDs.printUnknownData(8, std::string(__func__) + " - 11");
        // }

        Structure structure = read_type_prefix();
        readPreamble();
        parseStructure(structure); // @todo push structure

        // structure = read_type_prefix();
        // readPreamble();
        // parseStructure(structure); // @todo push structure
    }

    const uint16_t len3 = mDs.readUint16();

    spdlog::debug("len3 = {}", len3);

    for(size_t i = 0u; i < len3; ++i)
    {
        // if(i > 0u)
        // {
        //     readPreamble();
        //     mDs.printUnknownData(8, std::string(__func__) + " - 11");
        // }

        Structure structure;

        if(i == 0u)
        {
            // @todo this is type_prefix_very_long()
            mDs.printUnknownData(47, std::string(__func__) + " - 11");
            structure = ToStructure(0x0d); // Parse package instance for now until type_prefix_very_long is implemented
        }
        else
        {
            structure = read_type_prefix();
        }


        readPreamble();
        parseStructure(structure); // @todo push structure

        // structure = read_type_prefix();
        // readPreamble();
        // parseStructure(structure); // @todo push structure
    }

    // readPreamble();
    // mDs.printUnknownData(12, std::string(__func__) + " - 12");

    mDs.printUnknownData(10, std::string(__func__) + " - 10");

    const uint16_t lenX = mDs.readUint16();

    spdlog::debug("lenX = {}", lenX);

    for(size_t i = 0u; i < lenX; ++i)
    {
        Structure structure = read_type_prefix();
        readPreamble();
        parseStructure(structure); // @todo push structure
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return obj;
}