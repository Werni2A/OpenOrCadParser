#ifndef STRUCTURE_H
#define STRUCTURE_H


#include <cstdint>
#include <ostream>
#include <string>


// @todo there are a few consecutive items missing
enum class Structure
{
    SthInPages0            =   2, // @todo Should be name something like Instance as it is used by all .*Inst structures and instiates the actual object. Could also be a Graphic struct, see ComponentType as it occurs in Graphics Directory.

    DsnStream              =   4, //!< Used in the 'DsnStream' stream //@todo figure out real name and rename it

    Properties             =   6, // @todo Probably PackageProperties or 'Cell' but already the one before should be the properties. Maybe a bit more generic such that this one works for all, packages, symbols,...

    SchLib                 =   9, //!< Used in the 'Schematic' stream //@todo figure out real name and rename it
    Page                   =  10, // @todo Probably Page in schematic

    HierarchicalBlock      =  12, // @todo Not sure and what is it called in the XSD? Rename it!
    PartInst               =  13, // @todo This needs verification

    T0x10                  =  16, // @todo Is this probably DrawnInst? Place a part in the schematic and this will also appear. Or rather PinInst

    WireScalar             =  20,
    WireBus                =  21,

    Port                   =  23, //!< Hierarchical port in schematic
    GeoDefinition          =  24, // @todo renaming it to Graphics or something similar would make sense

    SymbolPinScalar        =  26,
    SymbolPinBus           =  27,

    BusEntry               =  29,

    T0x1f                  =  31, // @todo rename
    PinIdxMapping          =  32,
    GlobalSymbol           =  33, //!< Power symbol in library
    PortSymbol             =  34, //!< Hierarchical port symbol in library
    OffPageSymbol          =  35, //!< Off-page connector symbol in library

    Global                 =  37, //!< Power symbol instance in schematic
    OffPageConnector       =  38, //!< Off-page connector instance in schematic
    SymbolDisplayProp      =  39,

    SymbolVector           =  48,
    Alias                  =  49, //!< Net alias

    GraphicBoxInst         =  55,
    GraphicLineInst        =  56,
    GraphicArcInst         =  57,
    GraphicEllipseInst     =  58,
    GraphicPolygonInst     =  59, // @todo Needs verification. I was not able to create one in the GUI but it would make sense to have it here
    GraphicPolylineInst    =  60,
    GraphicCommentTextInst =  61,
    GraphicBitMapInst      =  62,

    TitleBlockSymbol       =  64, //!< Title block symbol in the library
    TitleBlock             =  65, //!< Title block instance in the schematic // @todo Not sure at all. This definetly needs verification. Then probably suffix with `Inst`?

    NetDbIdMapping         =  67,

    ERCSymbol              =  75, //!< ERC symbol in the library
    BookMarkSymbol         =  76, //!< Book mark symbol in the library
    ERCSymbolInst          =  77, //!< ERC marker instance in the schematic
    BookMarkSymbolInst     =  78, //!< Book mark instance in the schematic

    GraphicBezierInst      =  88,
    GraphicOleEmbedInst    =  89, //!< Embedds a file into the schematic, see https://en.wikipedia.org/wiki/Object_Linking_and_Embedding

    PinShapeSymbol         =  98,

    NetGroup               = 103  //!< Specifies which wires/busses belong to a given net group
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