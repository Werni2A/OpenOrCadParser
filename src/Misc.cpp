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

#include "Enums/GeometryStructure.hpp"
#include "Files/Package.hpp"
#include "Files/Symbol.hpp"
#include "Parser.hpp"
#include "Structures/CommentText.hpp"
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

    const int64_t idx = static_cast<int64_t>(nameIdx) - 1;

    std::string name;

    if(idx >= 0)
    {
        // Retrieve string from the library.
        name = mLibrary->symbolsLibrary.strLst.at(idx);
        // @todo provide try catch block for better exception messages
    }
    else if(idx == -1)
    {
        // Special case that is treated as an empty string.
        name = "";
    }
    else // idx < -1
    {
        // This should never happen.
        throw std::runtime_error(std::string(__func__) + ": Unexpected index " + std::to_string(idx));
    }

    return name;
}


std::string SymbolUserProp::getVal() const
{
    if(mLibrary == nullptr)
    {
        return "";
    }

    const int64_t idx = static_cast<int64_t>(valIdx) - 1;

    std::string val;

    if(idx >= 0)
    {
        // Retrieve string from the library.
        val = mLibrary->symbolsLibrary.strLst.at(idx);
        // @todo provide try catch block for better exception messages
    }
    else if(idx == -1)
    {
        // Special case that is treated as an empty string.
        val = "";
    }
    else // idx < -1
    {
        // This should never happen.
        throw std::runtime_error(std::string(__func__) + ": Unexpected index " + std::to_string(idx));
    }

    return val;
}


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
        if(i > 0u)
        {
            readPreamble();
        }

        GeometryStructure geoStruct = ToGeometryStructure(mDs.readUint16());
        readGeometryStructure(geoStruct);
    }

    readPreamble();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 2");
    mDs.printUnknownData(6, std::string(__func__) + " - 3");

    const uint16_t followingLen = mDs.readUint16();

    for(size_t i = 0u; i < followingLen; ++i)
    {
        // const Structure structure = read_prefixes(3);
        const Structure structure = auto_read_prefixes();
        readConditionalPreamble(structure);
        parseStructure(structure);
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}


GeometrySpecification Parser::parseGlobalSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    GeometrySpecification obj = parseGeometrySpecification();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    // spdlog::info(to_string(obj));

    return obj;
}


GeometrySpecification Parser::parseSymbolHierarchic()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    GeometrySpecification obj = parseGeometrySpecification();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    // spdlog::info(to_string(obj));

    return obj;
}


GeometrySpecification Parser::parseOffPageSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    GeometrySpecification obj = parseGeometrySpecification();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    // spdlog::info(to_string(obj));

    return obj;
}


GeometrySpecification Parser::readPinShapeSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    GeometrySpecification obj = parseGeometrySpecification();

    // sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    // spdlog::info(to_string(obj));

    return obj;
}


void Parser::pushStructure(const std::pair<Structure, std::any>& structure, Package& container)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    switch(structure.first)
    {
        case Structure::Properties:        container.properties.push_back(std::any_cast<Properties>(structure.second));                        break;
        case Structure::GeoDefinition:     container.geometrySpecifications.push_back(std::any_cast<GeometrySpecification>(structure.second)); break;
        case Structure::SymbolPinScalar:   container.symbolPinScalars.push_back(std::any_cast<SymbolPinScalar>(structure.second));             break;
        case Structure::T0x1f:             container.t0x1fs.push_back(std::any_cast<T0x1f>(structure.second));                                 break;
        case Structure::PinIdxMapping:     container.pinIdxMappings.push_back(std::any_cast<PinIdxMapping>(structure.second));                 break;
        case Structure::GlobalSymbol:      container.globalSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));          break;
        case Structure::PortSymbol:        container.portSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));            break;
        case Structure::OffPageSymbol:     container.offPageSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));         break;
        case Structure::SymbolDisplayProp: container.symbolDisplayProps.push_back(std::any_cast<SymbolDisplayProp>(structure.second));         break;
        case Structure::SymbolVector:      container.symbolVectors.push_back(std::any_cast<GeometrySpecification>(structure.second));          break;
        case Structure::TitleBlockSymbol:  container.titleBlockSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));      break;
        case Structure::ERCSymbol:         container.ercSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));             break;
        case Structure::PinShapeSymbol:    container.pinShapeSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));        break;
        default: throw std::runtime_error("Structure " + to_string(structure.first) + " is not yet handled by " + __func__ + "!"); break;
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}


// @todo This parses the ERC file, move it to a separate file or combine with parseSymbol
// @todo return real data object
bool Parser::parseSymbolsERC()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    bool obj = false;

    // @todo Should I introduce something like read_type_prefix_very_long()?
    mDs.assumeData({0x4b}, std::string(__func__) + " - 0"); // Proably stands for ERC, see Structure.hpp 0x4b

    mDs.printUnknownData(8, std::string(__func__) + " - 1");

    // Structure structure = read_prefixes(4);
    Structure structure = auto_read_prefixes();

    spdlog::critical("{}", to_string(structure));

    readConditionalPreamble(structure);
    parseStructure(structure); // @todo push structure

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return obj;
}


void Parser::pushStructure(const std::pair<Structure, std::any>& structure, Symbol& container)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    switch(structure.first)
    {
        case Structure::Properties:        container.properties.push_back(std::any_cast<Properties>(structure.second));                        break;
        case Structure::GeoDefinition:     container.geometrySpecifications.push_back(std::any_cast<GeometrySpecification>(structure.second)); break;
        case Structure::SymbolPinScalar:   container.symbolPinScalars.push_back(std::any_cast<SymbolPinScalar>(structure.second));             break;
        case Structure::T0x1f:             container.t0x1fs.push_back(std::any_cast<T0x1f>(structure.second));                                 break;
        case Structure::PinIdxMapping:     container.pinIdxMappings.push_back(std::any_cast<PinIdxMapping>(structure.second));                 break;
        case Structure::GlobalSymbol:      container.globalSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));          break;
        case Structure::PortSymbol:        container.portSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));            break;
        case Structure::OffPageSymbol:     container.offPageSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));         break;
        case Structure::SymbolDisplayProp: container.symbolDisplayProps.push_back(std::any_cast<SymbolDisplayProp>(structure.second));         break;
        case Structure::SymbolVector:      container.symbolVectors.push_back(std::any_cast<GeometrySpecification>(structure.second));          break;
        case Structure::TitleBlockSymbol:  /*container.titleBlockSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second)); */ break;
        case Structure::ERCSymbol:         container.ercSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));             break;
        case Structure::PinShapeSymbol:    container.pinShapeSymbols.push_back(std::any_cast<GeometrySpecification>(structure.second));        break;
        default: throw std::runtime_error("Structure " + to_string(structure.first) + " is not yet handled by " + __func__ + "!"); break;
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}