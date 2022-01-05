#ifndef STRUCTURE_H
#define STRUCTURE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


enum class Structure : uint8_t
{
    SthInPages0            = 0x02, // @todo Should be name something like Instance as it is used by all .*Inst structures and instiates the actual object
    DsnStream              = 0x04, //!< Used in the 'DsnStream' stream //@todo figure out real name and rename it
    Properties             = 0x06, // @todo Probably PackageProperties or 'Cell' but already the one before should be the properties. Maybe a bit more generic such that this one works for all, packages, symbols,...
    SchLib                 = 0x09, //!< Used in the 'Schematic' stream //@todo figure out real name and rename it
    Page                   = 0x0a, // @todo Probably Page in schematic
    HierarchicalBlock      = 0x0c, // @todo Not sure and what is it called in the XSD? Rename it!
    PartInst               = 0x0d, // @todo This needs verification
    T0x10                  = 0x10, // @todo Is this probably DrawnInst? Place a part in the schematic and this will also appear. Or rather PinInst
    WireScalar             = 0x14,
    WireBus                = 0x15,
    Port                   = 0x17, //!< Hierarchical port in schematic
    GeoDefinition          = 0x18, // @todo renaming it to Graphics or something similar would make sense
    SymbolPinScalar        = 0x1a,
    SymbolPinBus           = 0x1b,
    BusEntry               = 0x1d,
    T0x1f                  = 0x1f, // @todo rename
    PinIdxMapping          = 0x20,
    GlobalSymbol           = 0x21, //!< Power symbol in library
    PortSymbol             = 0x22, //!< Hierarchical port symbol in library
    OffPageSymbol          = 0x23, //!< Off-page connector symbol in library
    Global                 = 0x25, //!< Power symbol instance in schematic
    OffPageConnector       = 0x26, //!< Off-page connector instance in schematic
    SymbolDisplayProp      = 0x27,
    SymbolVector           = 0x30,
    Alias                  = 0x31, //!< Net alias
    GraphicBoxInst         = 0x37,
    GraphicLineInst        = 0x38,
    GraphicArcInst         = 0x39,
    GraphicEllipseInst     = 0x3a,
    GraphicPolygonInst     = 0x3b, // @todo Needs verification. I was not able to create one in the GUI but it would make sense to have it here
    GraphicPolylineInst    = 0x3c,
    GraphicCommentTextInst = 0x3d,
    GraphicBitMapInst      = 0x3e,
    TitleBlockSymbol       = 0x40, //!< Title block symbol in the library
    TitleBlock             = 0x41, //!< Title block instance in the schematic // @todo Not sure at all. This definetly needs verification
    NetDbIdMapping         = 0x43,
    ERCSymbol              = 0x4b, //!< ERC symbol in the library
    ERC                    = 0x4d, //!< ERC marker in the schematic
    BookMarkSymbol         = 0x4e, //!< Book mark instance in the schematic
    GraphicBezierInst      = 0x58,
    GraphicOleEmbedInst    = 0x59, //!< Embedds a file into the schematic, see https://en.wikipedia.org/wiki/Object_Linking_and_Embedding
    PinShapeSymbol         = 0x62,
    NetBundleMapData       = 0x67  //!< Used in the 'NetBundleMapData' stream //@todo figure out real name and rename it. I think they can be created with NetGroups where we assign a net to a group. Maybe this mapping is represented with this structure
};


[[maybe_unused]]
static Structure ToStructure(uint8_t val)
{
    Structure structure;

    switch(val)
    {
        case 0x02: structure = Structure::SthInPages0;            break;
        case 0x04: structure = Structure::DsnStream;              break;
        case 0x06: structure = Structure::Properties;             break;
        case 0x09: structure = Structure::SchLib;                 break;
        case 0x0a: structure = Structure::Page;                   break;
        case 0x0c: structure = Structure::HierarchicalBlock;      break;
        case 0x0d: structure = Structure::PartInst;               break;
        case 0x10: structure = Structure::T0x10;                  break;
        case 0x14: structure = Structure::WireScalar;             break;
        case 0x15: structure = Structure::WireBus;                break;
        case 0x17: structure = Structure::Port;                   break;
        case 0x18: structure = Structure::GeoDefinition;          break;
        case 0x1a: structure = Structure::SymbolPinScalar;        break;
        case 0x1b: structure = Structure::SymbolPinBus;           break;
        case 0x1d: structure = Structure::BusEntry;               break;
        case 0x1f: structure = Structure::T0x1f;                  break;
        case 0x20: structure = Structure::PinIdxMapping;          break;
        case 0x21: structure = Structure::GlobalSymbol;           break;
        case 0x22: structure = Structure::PortSymbol;             break;
        case 0x23: structure = Structure::OffPageSymbol;          break;
        case 0x25: structure = Structure::Global;                 break;
        case 0x26: structure = Structure::OffPageConnector;       break;
        case 0x27: structure = Structure::SymbolDisplayProp;      break;
        case 0x30: structure = Structure::SymbolVector;           break;
        case 0x31: structure = Structure::Alias;                  break;
        case 0x37: structure = Structure::GraphicBoxInst;         break;
        case 0x38: structure = Structure::GraphicLineInst;        break;
        case 0x39: structure = Structure::GraphicArcInst;         break;
        case 0x3a: structure = Structure::GraphicEllipseInst;     break;
        case 0x3b: structure = Structure::GraphicPolygonInst;     break;
        case 0x3c: structure = Structure::GraphicPolylineInst;    break;
        case 0x3d: structure = Structure::GraphicCommentTextInst; break;
        case 0x3e: structure = Structure::GraphicBitMapInst;      break;
        case 0x40: structure = Structure::TitleBlockSymbol;       break;
        case 0x41: structure = Structure::TitleBlock;             break;
        case 0x43: structure = Structure::NetDbIdMapping;         break;
        case 0x4b: structure = Structure::ERCSymbol;              break;
        case 0x4d: structure = Structure::ERC;                    break;
        case 0x4e: structure = Structure::BookMarkSymbol;         break;
        case 0x58: structure = Structure::GraphicBezierInst;      break;
        case 0x59: structure = Structure::GraphicOleEmbedInst;    break;
        case 0x62: structure = Structure::PinShapeSymbol;         break;
        case 0x67: structure = Structure::NetBundleMapData;       break;
        default:
            std::string errorMsg = "Structure with value 0x" + ToHex(val, 2)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return structure;
}


[[maybe_unused]]
static std::string to_string(const Structure& structure)
{
    std::string str;

    switch(structure)
    {
        case Structure::SthInPages0:            str = "SthInPages0";            break;
        case Structure::DsnStream:              str = "DsnStream";              break;
        case Structure::Properties:             str = "Properties";             break;
        case Structure::SchLib:                 str = "SchLib";                 break;
        case Structure::Page:                   str = "Page";                   break;
        case Structure::HierarchicalBlock:      str = "HierarchicalBlock";      break;
        case Structure::PartInst:               str = "PartInst";               break;
        case Structure::T0x10:                  str = "T0x10";                  break;
        case Structure::WireScalar:             str = "WireScalar";             break;
        case Structure::WireBus:                str = "WireBus";                break;
        case Structure::Port:                   str = "Port";                   break;
        case Structure::GeoDefinition:          str = "GeoDefinition";          break;
        case Structure::SymbolPinScalar:        str = "SymbolPinScalar";        break;
        case Structure::SymbolPinBus:           str = "SymbolPinBus";           break;
        case Structure::BusEntry:               str = "BusEntry";               break;
        case Structure::T0x1f:                  str = "T0x1f";                  break;
        case Structure::PinIdxMapping:          str = "PinIdxMapping";          break;
        case Structure::GlobalSymbol:           str = "GlobalSymbol";           break;
        case Structure::PortSymbol:             str = "PortSymbol";             break;
        case Structure::OffPageSymbol:          str = "OffPageSymbol";          break;
        case Structure::Global:                 str = "Global";                 break;
        case Structure::OffPageConnector:       str = "OffPageConnector";       break;
        case Structure::SymbolDisplayProp:      str = "SymbolDisplayProp";      break;
        case Structure::SymbolVector:           str = "SymbolVector";           break;
        case Structure::Alias:                  str = "Alias";                  break;
        case Structure::GraphicBoxInst:         str = "GraphicBoxInst";         break;
        case Structure::GraphicLineInst:        str = "GraphicLineInst";        break;
        case Structure::GraphicArcInst:         str = "GraphicArcInst";         break;
        case Structure::GraphicEllipseInst:     str = "GraphicEllipseInst";     break;
        case Structure::GraphicPolygonInst:     str = "GraphicPolygonInst";     break;
        case Structure::GraphicPolylineInst:    str = "GraphicPolylineInst";    break;
        case Structure::GraphicCommentTextInst: str = "GraphicCommentTextInst"; break;
        case Structure::GraphicBitMapInst:      str = "GraphicBitMapInst";      break;
        case Structure::TitleBlockSymbol:       str = "TitleBlockSymbol";       break;
        case Structure::TitleBlock:             str = "TitleBlock";             break;
        case Structure::NetDbIdMapping:         str = "NetDbIdMapping";         break;
        case Structure::ERCSymbol:              str = "ERCSymbol";              break;
        case Structure::ERC:                    str = "ERC";                    break;
        case Structure::BookMarkSymbol:         str = "BookMarkSymbol";         break;
        case Structure::GraphicBezierInst:      str = "GraphicBezierInst";      break;
        case Structure::GraphicOleEmbedInst:    str = "GraphicOleEmbedInst";    break;
        case Structure::PinShapeSymbol:         str = "PinShapeSymbol";         break;
        case Structure::NetBundleMapData:       str = "NetBundleMapData";       break;
        default:
            std::string errorMsg = "Structure with value 0x" + ToHex(structure, 2)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Structure& structure)
{
    os << to_string(structure);
    return os;
}


#endif // STRUCTURE_H