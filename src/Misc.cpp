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

#include "Enums/Primitive.hpp"
#include "Primitives/PrimCommentText.hpp"
#include "Streams/StreamPackage.hpp"
#include "Streams/StreamSymbol.hpp"


struct SymbolUserProp
{
    uint32_t nameIdx;
    uint32_t valIdx;

    SymbolUserProp()
    { }

    std::string getName() const;
    std::string getVal() const;
};


std::string SymbolUserProp::getName() const
{
    std::string name;

    if(nameIdx >= 0)
    {
        // Retrieve string from the library.
        name = gLibrary->library->strLst.at(nameIdx);
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
        throw std::runtime_error(getMethodName(this, __func__) + ": Unexpected index " + std::to_string(nameIdx));
    }

    return name;
}


std::string SymbolUserProp::getVal() const
{
    std::string val;

    if(valIdx >= 0)
    {
        // Retrieve string from the library.
        val = gLibrary->library->strLst.at(valIdx);
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
        throw std::runtime_error(getMethodName(this, __func__) + ": Unexpected index " + std::to_string(valIdx));
    }

    return val;
}


// @todo this is a whole file parser. Split it up into the title block structure and move the rest to the symbol parser?
void Parser::readTitleBlockSymbol()
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    ds.printUnknownData(36, getMethodName(this, __func__) + ": 0");

    std::vector<SymbolUserProp> symbolUserProps; // @todo store in symbol

    const uint16_t propertyLen = ds.readUint16();

    for(size_t i = 0u; i < propertyLen; ++i)
    {
        SymbolUserProp symbolUserProp{};

        symbolUserProp.nameIdx = ds.readUint32(); // @todo move to Kaitai OrCAD: 'Symbol Properties' (Fixed value on the left)
        symbolUserProp.valIdx  = ds.readUint32(); // @todo move to Kaitai OrCAD: 'Symbol Properties' (Adjustable value on the right)

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
    std::string str0 = ds.readStringLenZeroTerm();

    ds.printUnknownData(7, getMethodName(this, __func__) + ": 1");

    const uint16_t someLen = ds.readUint16();

    spdlog::trace("someLen = {}", someLen);

    for(size_t i = 0u; i < someLen; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();
        readPrimitive(primitive);
    }

    ds.assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 2");
    ds.printUnknownData(6, getMethodName(this, __func__) + ": 3");

    const uint16_t followingLen = ds.readUint16();

    spdlog::trace("followingLen = {}", followingLen);

    for(size_t i = 0u; i < followingLen; ++i)
    {
        // @todo push structure
        const auto s = readStructure();
        if(s)
        {
            spdlog::debug("VERIFYING Misc Structure0 is {}", NAMEOF_TYPE_RTTI(*s));
        }
    }

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
}