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

#include "../Parser.hpp"
#include "../Structures/SymbolsLibrary.hpp"


SymbolsLibrary Parser::parseSymbolsLibrary()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    size_t startOffset = mDs.getCurrentOffset();

    SymbolsLibrary symbolsLibrary;

    symbolsLibrary.introduction = readStringZeroTerm();
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
    padRest(startOffset, 32);

    assume_data({0x03, 0x00, 0x02, 0x00}, std::string(__func__) + " - 0");

    symbolsLibrary.createDate = static_cast<time_t>(mDs.readUint32());
    symbolsLibrary.modifyDate = static_cast<time_t>(mDs.readUint32());

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");

    const uint16_t textFontLen = mDs.readUint16();

    if(textFontLen == 0u)
    {
        throw std::runtime_error("textFontLen == 0 doesn't make sense as it causes an succeeding underflow!");
    }

    for(int i = 0; i < static_cast<int>(textFontLen) - 1; ++i)
    {
        symbolsLibrary.textFonts.push_back(readTextFont());
    }

    // Even this big chunk of data seems to be constant
    // assume_data(
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
    std::clog << print_unknown_data(58, std::string(__func__) + " - 2") << std::endl;

    // Looks like this has always the same size as it consists of
    // the mandatory package fields '1ST PART FIELD' up to the
    // '7TH PART FIELD' plus 'PCB Footprint'.
    for(size_t i = 0u; i < 8u; ++i)
    {
        symbolsLibrary.strLstPartField.push_back(readStringBothTerm());
    }

    // Even this big chunk of data seems to be constant
    std::cout << print_unknown_data(156, std::string(__func__) + " - 3") << std::endl;

    const uint32_t strLstLen = mDs.readUint32();

    assume_data({0x00, 0x00, 0x00}, std::string(__func__) + " - 4");

    for(size_t i = 0u; i < strLstLen - 1; ++i)
    {
        symbolsLibrary.strLst.push_back(readStringBothTerm());
    }

    const uint16_t aliasLstLen = mDs.readUint16();

    for(size_t i = 0u; i < aliasLstLen; ++i)
    {
        std::string alias   = readStringBothTerm();
        std::string package = readStringBothTerm();
        symbolsLibrary.partAliases.push_back(std::make_pair(alias, package));
    }

    if(mFileType == FileType::Schematic)
    {
        std::clog << print_unknown_data(8, std::string(__func__) + " - 5") << std::endl;
        std::string schematicName = readStringBothTerm();
        std::clog << schematicName << std::endl;
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbolsLibrary << std::endl;

    return symbolsLibrary;
}


void Parser::parseSchematic()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    Structure structure = read_type_prefix_short();

    if(structure != Structure::SchLib)
    {
        // @todo throw some exception
    }
    // std::clog << print_unknown_data(12, std::string(__func__) + " - 0") << std::endl;

    readPreamble();

    std::string schematic_name = readStringBothTerm();

    std::clog << print_unknown_data(4, std::string(__func__) + " - 1") << std::endl;

    const uint16_t schematicPages = mDs.readUint16();

    for(size_t i = 0u; i < schematicPages; ++i)
    {
        std::string page_name = readStringBothTerm();
        std::cout << page_name << std::endl;
    }

    const uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        std::clog << print_unknown_data(4, std::string(__func__) + " - 1") << std::endl;
    }

    const uint16_t len2 = mDs.readUint16();

    for(size_t i = 0u; i < len2; ++i)
    {
        std::clog << print_unknown_data(5, std::string(__func__) + " - 2") << std::endl;
    }

    std::clog << print_unknown_data(4, std::string(__func__) + " - 3") << std::endl;

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::parseHierarchy()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    std::clog << print_unknown_data(9, std::string(__func__) + " - 0") << std::endl;

    std::string schematicName = readStringBothTerm();

    std::clog << print_unknown_data(9, std::string(__func__) + " - 1") << std::endl;

    const uint16_t netLen = mDs.readUint16();

    for(size_t i = 0u; i < netLen; ++i)
    {
        const Structure structure = read_type_prefix_short();

        readPreamble(); // @tood make conditional preamble

        // @todo Move the following data into a own structore for the specific Structure type

        uint32_t dbId = mDs.readUint32();

        std::string name = readStringBothTerm(); // net name
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::parseSymbolsERC()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    // @todo Should I introduce something like read_type_prefix_very_long()?
    assume_data({0x4b}, std::string(__func__) + " - 0"); // Proably stands for ERC

    std::clog << print_unknown_data(8, std::string(__func__) + " - 1");

    Structure structure = read_type_prefix_long();
    readConditionalPreamble(structure);
    parseStructure(structure); // @todo push structure

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}