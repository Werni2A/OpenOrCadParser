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

#include "PageSettings.hpp"
#include "Streams/StreamLibrary.hpp"
#include "Win32/LOGFONTA.hpp"


void StreamLibrary::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    size_t startOffset = mDs.get().getCurrentOffset();

    introduction = mDs.get().readStringZeroTerm();
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
    mDs.get().padRest(startOffset, 32, false);

    spdlog::trace("introduction = {}", introduction);

    // I saw versions:
    // 2.0; 3.2; 3.3
    const uint16_t versionMajor = mDs.get().readUint16();
    const uint16_t versionMinor = mDs.get().readUint16();

    spdlog::trace("version = {}.{}", versionMajor, versionMinor);

    createDate = static_cast<time_t>(mDs.get().readUint32());
    modifyDate = static_cast<time_t>(mDs.get().readUint32());

    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");

    // @todo the GUI specifies 15 fonts under
    //       `Options` -> `Design Templates...` -> `Fonts`
    //       is there some correlation?
    const uint16_t textFontLen = mDs.get().readUint16();

    spdlog::trace("textFontLen = {}", textFontLen);

    if(textFontLen == 0u)
    {
        throw std::runtime_error("textFontLen == 0 doesn't make sense as it causes an succeeding underflow!");
    }

    for(int i = 0; i < static_cast<int>(textFontLen) - 1; ++i)
    {
        LOGFONTA font{};
        font.read(mDs);
        textFonts.push_back(font);
    }

    const uint16_t someLen = mDs.get().readUint16();
    for(int i = 0; i < someLen; ++i)
    {
        const uint16_t someData = mDs.get().readUint16();
    }

    mDs.get().printUnknownData(4, std::string(__func__) + " - 2.0");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 2.1");

    // Property to Part Field Mapping
    // See OrCAD: `Options` -> `Design Template...` -> `SDT Compatibility`
    //            -> `Property to Part Field Mapping`
    const size_t partFields = 8U; // `Part Field 1` up to `Part Field 8`
    for(size_t i = 0u; i < partFields; ++i)
    {
        strLstPartField.push_back(mDs.get().readStringLenZeroTerm());
    }

    pageSettings.read();

    // @todo sometimes it's 2 and sometimes 4 byte... Looks like a data format change
    // const uint32_t strLstLen = mDs.readUint16();
    const uint32_t strLstLen = mDs.get().readUint32();

    for(size_t i = 0u; i < strLstLen; ++i)
    {
        strLst.push_back(mDs.get().readStringLenZeroTerm());
    }

    const uint16_t aliasLstLen = mDs.get().readUint16();

    for(size_t i = 0u; i < aliasLstLen; ++i)
    {
        std::string alias   = mDs.get().readStringLenZeroTerm();
        std::string package = mDs.get().readStringLenZeroTerm();
        partAliases.push_back(std::make_pair(alias, package));
    }

    if(gFileType == FileType::Schematic)
    {
        mDs.get().printUnknownData(8, std::string(__func__) + " - 5");
        std::string schematicName = mDs.get().readStringLenZeroTerm();
        spdlog::trace("schematicName = {}", schematicName);
    }

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}