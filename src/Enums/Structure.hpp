#ifndef STRUCTURE_H
#define STRUCTURE_H


#include <cstdint>
#include <ostream>
#include <string>


enum class Structure
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
static constexpr Structure ToStructure(uint8_t aVal)
{
    return ToEnum<Structure, decltype(aVal)>(aVal);
}


[[maybe_unused]]
static std::string to_string(const Structure& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Structure& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // STRUCTURE_H