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
    // is copied into. However, when the string does not require the
    // full buffer size it contains still data from the previous
    // application. When the buffer is written into the file this
    // probably causes some privacy issues as the data at the end
    // of the buffer should never end up in the symbols file.
    // Some OrCAD code like this would cause the issue:
    // char buffer[32];
    // strcpy(buffer, srcStr);
    // write_data_to_file(buffer, sizeof(buffer));
    mDs.padRest(startOffset, 32, false);

    spdlog::debug("introduction = {}", obj.introduction);

    // I saw versions:
    // 2.0; 3.2; 3.3
    const uint16_t versionMajor = mDs.readUint16();
    const uint16_t versionMinor = mDs.readUint16();

    spdlog::debug("version = {}.{}", versionMajor, versionMinor);

    obj.createDate = static_cast<time_t>(mDs.readUint32());
    obj.modifyDate = static_cast<time_t>(mDs.readUint32());

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");

    // @todo the GUI specifies 15 fonts under
    //       `Options` -> `Design Templates...` -> `Fonts`
    //       is there some correlation?
    const uint16_t textFontLen = mDs.readUint16();

    spdlog::debug("textFontLen = {}", textFontLen);

    if(textFontLen == 0u)
    {
        throw std::runtime_error("textFontLen == 0 doesn't make sense as it causes an succeeding underflow!");
    }

    for(int i = 0; i < static_cast<int>(textFontLen) - 1; ++i)
    {
        obj.textFonts.push_back(readTextFont());
    }

    const uint16_t someLen = mDs.readUint16();
    for(int i = 0; i < someLen; ++i)
    {
        const uint16_t someData = mDs.readUint16();
    }

    mDs.printUnknownData(4, std::string(__func__) + " - 2.0");
    mDs.printUnknownData(4, std::string(__func__) + " - 2.1");

    // Property to Part Field Mapping
    // See OrCAD: `Options` -> `Design Template...` -> `SDT Compatibility`
    //            -> `Property to Part Field Mapping`
    const size_t partFields = 8U; // `Part Field 1` up to `Part Field 8`
    for(size_t i = 0u; i < partFields; ++i)
    {
        obj.strLstPartField.push_back(mDs.readStringLenZeroTerm());
    }

    // Even this big chunk of data seems to be constant
    mDs.printUnknownData(156, std::string(__func__) + " - 3");

    // @todo sometimes it's 2 and sometimes 4 byte... Looks like a data format change
    // const uint32_t strLstLen = mDs.readUint16();
    const uint32_t strLstLen = mDs.readUint32();

    for(size_t i = 0u; i < strLstLen; ++i)
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