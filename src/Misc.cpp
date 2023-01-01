#include <algorithm>
#include <any>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

#include "DataVariants.hpp"
#include "Enums/Primitive.hpp"
#include "Parser.hpp"
#include "Primitives/PrimCommentText.hpp"
#include "Streams/StreamPackage.hpp"
#include "Streams/StreamSymbol.hpp"
#include "Structures/TextFont.hpp"


struct SymbolUserProp
{
    uint32_t nameIdx;
    uint32_t valIdx;

    const Library* mLibrary;

    SymbolUserProp(const Library* aLibrary) : mLibrary(aLibrary) {}
    SymbolUserProp() : SymbolUserProp{nullptr} {}

    std::string getName() const;
    std::string getVal() const;
};


std::string SymbolUserProp::getName() const
{
    if(mLibrary == nullptr)
    {
        return "";
    }

    std::string name;

    if(nameIdx >= 0)
    {
        // Retrieve string from the library.
        name = mLibrary->library.strLst.at(nameIdx);
        // @todo provide try catch block for better exception messages
    }
    else if(nameIdx == -1U)
    {
        // Special case that is treated as an empty string.
        name = "";
    }
    else // nameIdx < -1
    {
        // This should never happen.
        throw std::runtime_error(std::string(__func__) + ": Unexpected index " + std::to_string(nameIdx));
    }

    return name;
}


std::string SymbolUserProp::getVal() const
{
    if(mLibrary == nullptr)
    {
        return "";
    }

    std::string val;

    if(valIdx >= 0)
    {
        // Retrieve string from the library.
        val = mLibrary->library.strLst.at(valIdx);
        // @todo provide try catch block for better exception messages
    }
    else if(valIdx == -1U)
    {
        // Special case that is treated as an empty string.
        val = "";
    }
    else // valIdx < -1
    {
        // This should never happen.
        throw std::runtime_error(std::string(__func__) + ": Unexpected index " + std::to_string(valIdx));
    }

    return val;
}


// @todo this is a whole file parser. Split it up into the title block structure and move the rest to the symbol parser?
void Parser::readTitleBlockSymbol()
{
    mDs.printUnknownData(36, std::string(__func__) + " - 0");

    std::vector<SymbolUserProp> symbolUserProps; // @todo store in symbol

    const uint16_t propertyLen = mDs.readUint16();

    for(size_t i = 0u; i < propertyLen; ++i)
    {
        SymbolUserProp symbolUserProp{&mLibrary};

        symbolUserProp.nameIdx = mDs.readUint32(); // @todo move to Kaitai OrCAD: 'Symbol Properties' (Fixed value on the left)
        symbolUserProp.valIdx  = mDs.readUint32(); // @todo move to Kaitai OrCAD: 'Symbol Properties' (Adjustable value on the right)

        symbolUserProps.push_back(symbolUserProp);
    }

    for(size_t i = 0u; i < symbolUserProps.size(); ++i)
    {
        std::string name = symbolUserProps[i].getName();
        std::string val  = symbolUserProps[i].getVal();

        spdlog::debug("{}: {} <- {}", i, name, val);
    }

    // The following should be its own structure
    readPreamble();
    std::string str0 = mDs.readStringLenZeroTerm();

    mDs.printUnknownData(7, std::string(__func__) + " - 1");

    const uint16_t someLen = mDs.readUint16();

    for(size_t i = 0u; i < someLen; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();
        readPrimitive(primitive);
    }

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 2");
    mDs.printUnknownData(6, std::string(__func__) + " - 3");

    const uint16_t followingLen = mDs.readUint16();

    for(size_t i = 0u; i < followingLen; ++i)
    {
        const Structure structure = auto_read_prefixes();
        readPreamble();
        readStructure(structure);
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}


StructPrimitives Parser::readStructGlobalSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructPrimitives obj = readStructPrimitives();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


StructPrimitives Parser::readStructHierarchicSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructPrimitives obj = readStructPrimitives();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


StructPrimitives Parser::readStructOffPageSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructPrimitives obj = readStructPrimitives();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


StructPrimitives Parser::readStructPinShapeSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructPrimitives obj = readStructPrimitives();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


// @todo This parses the ERC file, move it to a separate file or combine with parseSymbol
// @todo return real data object
bool Parser::readStreamERC()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    bool obj = false;

    // Structure structure = read_prefixes(5);
    Structure structure = auto_read_prefixes();

    readPreamble();

    readStructure(structure); // @todo push structure

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return obj;
}