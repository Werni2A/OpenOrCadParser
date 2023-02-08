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


void StreamLibrary::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion();
    }

    size_t startOffset = ds.getCurrentOffset();

    introduction = ds.readStringZeroTerm();
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
    ds.padRest(startOffset, 32, false);

    spdlog::trace("introduction = {}", introduction);

    // I saw versions:
    // 2.0; 3.2; 3.3
    const uint16_t versionMajor = ds.readUint16();
    const uint16_t versionMinor = ds.readUint16();

    spdlog::trace("version = {}.{}", versionMajor, versionMinor);

    createDate = static_cast<time_t>(ds.readUint32());
    modifyDate = static_cast<time_t>(ds.readUint32());

    ds.assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 1");

    // @todo the GUI specifies 15 fonts under
    //       `Options` -> `Design Templates...` -> `Fonts`
    //       is there some correlation?
    const uint16_t textFontLen = ds.readUint16();

    spdlog::trace("textFontLen = {}", textFontLen);

    if(textFontLen == 0u)
    {
        throw std::runtime_error("textFontLen == 0 doesn't make sense as it causes an succeeding underflow!");
    }

    for(int i = 0; i < static_cast<int>(textFontLen) - 1; ++i)
    {
        LOGFONTA font{};
        font.read(mCtx);
        textFonts.push_back(font);
    }

    const uint16_t someLen = ds.readUint16();

    spdlog::trace("someLen = {}", someLen);

    for(int i = 0; i < someLen; ++i)
    {
        const uint16_t someData = ds.readUint16();

        spdlog::trace("someData [{}] = {}", i, someData);
    }

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 2.0");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 2.1");

    // Property to Part Field Mapping
    // See OrCAD: `Options` -> `Design Template...` -> `SDT Compatibility`
    //            -> `Property to Part Field Mapping`
    const size_t partFields = 8U; // `Part Field 1` up to `Part Field 8`
    for(size_t i = 0u; i < partFields; ++i)
    {
        strLstPartField.push_back(ds.readStringLenZeroTerm());
    }

    pageSettings.read();

    uint32_t strLstLen = 0U;

    // @todo Versions were chosen randomly
    if(aVersion == FileFormatVersion::A)
    {
        strLstLen = ds.readUint16();
    }
    else
    {
        strLstLen = ds.readUint32();
    }

    for(size_t i = 0u; i < strLstLen; ++i)
    {
        strLst.push_back(ds.readStringLenZeroTerm());
    }

    const uint16_t aliasLstLen = ds.readUint16();

    for(size_t i = 0u; i < aliasLstLen; ++i)
    {
        std::string alias   = ds.readStringLenZeroTerm();
        std::string package = ds.readStringLenZeroTerm();
        partAliases.push_back(std::make_pair(alias, package));
    }

    if(gFileType == FileType::Schematic)
    {
        ds.printUnknownData(8, getMethodName(this, __func__) + ": 5");
        std::string schematicName = ds.readStringLenZeroTerm();
        spdlog::trace("schematicName = {}", schematicName);
    }

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}