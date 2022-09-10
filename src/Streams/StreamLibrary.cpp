#include <cassert>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

#include "Parser.hpp"
#include "Streams/StreamLibrary.hpp"
#include "Structures/TextFont.hpp"


StreamLibrary Parser::readStreamLibrary()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    size_t startOffset = mDs.getCurrentOffset();

    StreamLibrary obj;

    obj.introduction = mDs.readStringZeroTerm();
    // Looks like OrCAD creates a fixed size buffer where the string
    // is copied into. However, when the string does not requrie the
    // full buffer size it contains still data from the previous
    // application. When the buffer is written into the file this
    // probably causes some privacy issues as the data at the end
    // of the buffer should never end up in the symbols file.
    // Some OrCAD code like this would cause the issue:
    // char buffer[32];
    // strcpy(buffer, srcStr);
    // write_data_to_file(buffer, sizeof(buffer));
    mDs.padRest(startOffset, 32, false);

    // @todo Probably file version? E.g. {0x03, 0x00, 0x02, 0x00} = 3.2
    //       I saw: 2.0
    //              3.2
    //              3.3
    // mDs.assumeData({0x03, 0x00, 0x02, 0x00}, std::string(__func__) + " - 0");
    mDs.printUnknownData(4, std::string(__func__) + " - 0 - Probably file/library version");

    obj.createDate = static_cast<time_t>(mDs.readUint32());
    obj.modifyDate = static_cast<time_t>(mDs.readUint32());

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");

    const uint16_t textFontLen = mDs.readUint16();

    if(textFontLen == 0u)
    {
        throw std::runtime_error("textFontLen == 0 doesn't make sense as it causes an succeeding underflow!");
    }

    for(int i = 0; i < static_cast<int>(textFontLen) - 1; ++i)
    {
        obj.textFonts.push_back(readTextFont());
    }

    // Even this big chunk of data seems to be constant
    // mDs.assumeData(
    //     { 0x18, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
    //       0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
    //       0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
    //       0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
    //       0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    //       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //       0x00, 0x00 },
    //     "parseSymbolsLibrary2");
    // I've seen:
    // 0x864: 18 00 01 00 01 00 01 00 00 00 00 00 01 00 01 00 | ................
    // 0x864: 01 00 01 00 01 00 01 00 01 00 01 00 02 00 01 00 | ................
    // 0x864: 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 | ................
    // 0x864: 00 00 00 00 00 00 00 00 00 00                   | ..........
    // as well as:
    // 0x864: 18 00 01 00 01 00 01 00 00 00 00 00 01 00 01 00 | ................
    // 0x864: 01 00 01 00 01 00 01 00 01 00 01 00 02 00 01 00 | ................
    // 0x864: 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 | ................
    // 0x864: 00 00 00 00 00 00 01 00 00 00                   | ..........
    mDs.printUnknownData(58, std::string(__func__) + " - 2");

    // Looks like this has always the same size as it consists of
    // the mandatory package fields '1ST PART FIELD' up to the
    // '7TH PART FIELD' plus 'PCB Footprint'.
    for(size_t i = 0u; i < 8u; ++i)
    {
        obj.strLstPartField.push_back(mDs.readStringLenZeroTerm());
    }

    // Even this big chunk of data seems to be constant
    mDs.printUnknownData(156, std::string(__func__) + " - 3");

    const uint32_t strLstLen = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 4");

    for(size_t i = 0u; i < strLstLen - 1; ++i)
    {
        obj.strLst.push_back(mDs.readStringLenZeroTerm());
    }

    const uint16_t aliasLstLen = mDs.readUint16();

    for(size_t i = 0u; i < aliasLstLen; ++i)
    {
        std::string alias   = mDs.readStringLenZeroTerm();
        std::string package = mDs.readStringLenZeroTerm();
        obj.partAliases.push_back(std::make_pair(alias, package));
    }

    if(mFileType == FileType::Schematic)
    {
        mDs.printUnknownData(8, std::string(__func__) + " - 5");
        std::string schematicName = mDs.readStringLenZeroTerm();
        spdlog::debug("schematicName = {}", schematicName);
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}