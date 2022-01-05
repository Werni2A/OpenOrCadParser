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
#include <vector>


#include "../Parser.hpp"
#include "../Structures/Package.hpp"


#include <utility>

#include "../Enums/GeometryStructure.hpp"






#include "../Structures/CommentText.hpp"

#include "../Structures/TextFont.hpp"
// #include "Library.hpp"


TextFont CommentText::getTextFont() const
{
    if(mLibrary == nullptr)
    {
        throw std::logic_error(std::string(__func__) + ": mLibrary should be set!");
    }

    const int64_t idx = static_cast<int64_t>(textFontIdx) - 1;

    TextFont textFont;

    if(idx >= 0)
    {
        // Retrieve font from the library.
        textFont = mLibrary->symbolsLibrary.textFonts.at(idx);
        // @todo provide try catch block for better exception messages
    }
    else if(idx == -1)
    {
        // @todo Unknown but it is probably the default font;
        throw std::runtime_error(std::string(__func__) + ": Check this out!");
    }
    else // idx < -1
    {
        // This should never happen.
        throw std::runtime_error(std::string(__func__) + ": Unexpected index " + std::to_string(idx));
    }

    return textFont;
}


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
    std::cout << print_unknown_data(36, std::string(__func__) + " - 0") << std::endl;

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

        std::cout << std::to_string(i) << ": " << name << " <- " << val << std::endl;
    }

    // The following should be its own structure
    readPreamble();
    std::string str0 = readStringBothTerm();

    std::cout << print_unknown_data(7, std::string(__func__) + " - 1") << std::endl;

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

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 2");
    std::cout << print_unknown_data(6, std::string(__func__) + " - 3");

    const uint16_t followingLen = mDs.readUint16();

    for(size_t i = 0u; i < followingLen; ++i)
    {
        const Structure structure = read_type_prefix();
        readConditionalPreamble(structure);
        parseStructure(structure);
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


GeometrySpecification Parser::parseGlobalSymbol()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    GeometrySpecification geometrySpecification = parseGeometrySpecification();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    // std::clog << geometrySpecification << std::endl;

    return geometrySpecification;
}


GeometrySpecification Parser::parseSymbolHierarchic()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    GeometrySpecification geometrySpecification = parseGeometrySpecification();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    // std::clog << geometrySpecification << std::endl;

    return geometrySpecification;
}


GeometrySpecification Parser::parseOffPageSymbol()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    GeometrySpecification geometrySpecification = parseGeometrySpecification();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    // std::clog << geometrySpecification << std::endl;

    return geometrySpecification;
}


GeometrySpecification Parser::readPinShapeSymbol()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    GeometrySpecification geometrySpecification = parseGeometrySpecification();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    // std::clog << geometrySpecification << std::endl;

    return geometrySpecification;
}


void Parser::pushStructure(const std::pair<Structure, std::any>& structure, Package& container)
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

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

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


Package Parser::parseSymbol()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    Package symbol; // @todo make to symbol

    Structure structure;

    Structure introId = ToStructure(mDs.readUint8());

    switch(introId)
    {
        case Structure::GlobalSymbol:
        case Structure::PortSymbol:
        case Structure::OffPageSymbol:
        case Structure::PinShapeSymbol:
            std::cout << print_unknown_data(2, std::string(__func__) + " - 0") << std::endl;
            assume_data({0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");
            structure = read_type_prefix_long();
            break;

        case Structure::TitleBlockSymbol:
            readTitleBlockSymbol();
            return symbol;
            break;

        default:
            throw std::runtime_error("This was unexpected in " + std::string(__func__));
            break;
    }

    readConditionalPreamble(introId);
    pushStructure(parseStructure(structure), symbol);

    readPreamble();

    if(introId == Structure::PinShapeSymbol)
    {
        readSymbolBBox(); // @todo push structure
    }
    else
    {
        std::cout << print_unknown_data(10, std::string(__func__) + " - 1.1") << std::endl;
    }

    // @todo how often does it repeat? This should be specified somewhere....
    for(size_t i = 0u; true; ++i)
    {
        if(mDs.isEoF())
        {
            std::cout << "i = " << std::to_string(i) << std::endl;
            break;
        }

        structure = read_type_prefix();
        readConditionalPreamble(structure);
        // readPreamble();
        pushStructure(parseStructure(structure), symbol);

        std::cout << print_unknown_data(2, std::string(__func__) + " - 2") << std::endl;
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbol << std::endl;

    return symbol;
}


Package Parser::parsePackage()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    Package package;

    Structure structure;

    const uint16_t sectionCount = mDs.readUint16();

    for(size_t i = 0u; i < sectionCount; ++i)
    {
        std::cout << "Marker 0" << std::endl;

        structure = read_type_prefix_long();
        readConditionalPreamble(structure);
        pushStructure(parseStructure(structure), package);

        std::cout << "Marker 1" << std::endl;

        structure = read_type_prefix();
        readConditionalPreamble(structure);
        std::cout << "Marker 1.5" << std::endl;
        pushStructure(parseStructure(structure), package);

        // if(structure == Structure::GeoDefinition && mFileFormatVersion == FileFormatVersion::C)
        //     discard_bytes(6);

        // structure = read_type_prefix();

        std::cout << "Marker 2" << std::endl;

        if(mFileFormatVersion == FileFormatVersion::B)
        {
            assume_data({0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");
            // std::cout << print_unknown_data(8, std::string(__func__) + " - 1");
        }
        else if(mFileFormatVersion >= FileFormatVersion::C)
        {
            readPreamble();
        }

        std::cout << "Marker 3" << std::endl;

        assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 2");
        std::cout << print_unknown_data(4, std::string(__func__) + " - 2") << std::endl;
        // std::cout << print_unknown_data(12, std::string(__func__) + " - 2") << std::endl;

        const uint16_t followingLen1 = mDs.readUint16();

        for(size_t i = 0u; i < followingLen1; ++i)
        {
            std::clog << "0x" << ToHex(mDs.getCurrentOffset(), 8) << ": followingLen1 Iteration "
                    << std::to_string(i + 1) << "/" << std::to_string(followingLen1) << std::endl;

            structure = read_type_prefix();
            readConditionalPreamble(structure);
            pushStructure(parseStructure(structure), package);
        }

        std::cout << "Marker 4" << std::endl;

        const uint16_t followingLen2 = mDs.readUint16();

        for(size_t i = 0u; i < followingLen2; ++i)
        {
            std::clog << "0x" << ToHex(mDs.getCurrentOffset(), 8) << ": followingLen2 Iteration "
                    << std::to_string(i + 1) << "/" << std::to_string(followingLen2) << std::endl;

            structure = read_type_prefix();
            readConditionalPreamble(structure);
            pushStructure(parseStructure(structure), package);
        }

        std::cout << "Marker 5" << std::endl;

        /*
        std::cout << print_unknown_data(22, std::string(__func__) + " - w") << std::endl;
        structure = read_type_prefix();
        readConditionalPreamble(structure);
        // readPreamble();
        */

        package.generalProperties = readGeneralProperties();

        std::cout << "Section count " << std::to_string(i) << " finished" << std::endl;
    }

    std::cout << "Marker 6" << std::endl;

    // @todo how often does it repeat? This should be specified somewhere....
    for(size_t i = 0u; true; ++i)
    {
        if(mDs.isEoF())
        {
            std::cout << "i = " << std::to_string(i) << std::endl;
            break;
        }

        std::cout << "Marker 7" << std::endl;

        if(i == 0u)
        {
            structure = read_type_prefix_long();
        }
        else
        {
            structure = read_type_prefix();
        }

        readConditionalPreamble(structure);
        pushStructure(parseStructure(structure), package);
    }

    std::cout << "Marker 8" << std::endl;

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << package << std::endl;

    return package;
}