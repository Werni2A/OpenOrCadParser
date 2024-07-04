#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"

namespace OOCP
{
// @todo there are a few consecutive items missing
enum class Structure
{
    SthInPages0 =
        2, // @todo Should be name something like Instance as it is used by all .*Inst structures and instiates the
           // actual object. Could also be a Graphic struct, see ComponentType as it occurs in Graphics Directory.

    DsnStream = 4, //!< Used in the 'DsnStream' stream //@todo figure out real name and rename it

    PartCell = 6,

    SchLib         = 9,  //!< Used in the 'Schematic' stream //@todo figure out real name and rename it
    Page           = 10, // @todo Probably Page in schematic
    PartInstance   = 11,
    DrawnInstance  = 12,
    PlacedInstance = 13,

    T0x10 = 16, // @todo Is this probably DrawnInst? Place a part in the schematic and this will also appear. Or rather
                // PinInst

    WireScalar = 20,
    WireBus    = 21,

    Port        = 23, //!< Hierarchical port in schematic
    LibraryPart = 24,

    SymbolPinScalar = 26,
    SymbolPinBus    = 27,

    BusEntry = 29,

    Package       = 31,
    Device        = 32,
    GlobalSymbol  = 33, //!< Power symbol in library
    PortSymbol    = 34, //!< Hierarchical port symbol in library
    OffPageSymbol = 35, //!< Off-page connector symbol in library

    Global            = 37, //!< Power symbol instance in schematic
    OffPageConnector  = 38, //!< Off-page connector instance in schematic
    SymbolDisplayProp = 39,

    SymbolVector = 48,
    Alias        = 49, //!< Net alias

    T0x34 = 52, // @todo rename
    T0x35 = 53, // @todo rename

    GraphicBoxInst     = 55,
    GraphicLineInst    = 56,
    GraphicArcInst     = 57,
    GraphicEllipseInst = 58,
    GraphicPolygonInst =
        59, // @todo Needs verification. I was not able to create one in the GUI but it would make sense to have it here
    GraphicPolylineInst    = 60,
    GraphicCommentTextInst = 61,
    GraphicBitMapInst      = 62,

    TitleBlockSymbol = 64, //!< Title block symbol in the library
    TitleBlock       = 65, //!< Title block instance in the schematic // @todo Not sure at all. This definetly needs
                           //!< verification. Then probably suffix with `Inst`?

    SthInHierarchy1 = 66, // @todo
    NetDbIdMapping  = 67,
    SthInHierarchy2 = 68, // @todo
    T0x45           = 69, // @todo

    ERCSymbol          = 75, //!< ERC symbol in the library
    BookMarkSymbol     = 76, //!< Book mark symbol in the library
    ERCObject          = 77, //!< ERC marker instance in the schematic
    BookMarkSymbolInst = 78, //!< Book mark instance in the schematic

    SthInHierarchy3 = 82, // @todo

    GraphicBezierInst = 88,
    GraphicOleEmbedInst =
        89, //!< Embedds a file into the schematic, see https://en.wikipedia.org/wiki/Object_Linking_and_Embedding

    T0x5b = 91,

    PinShapeSymbol = 98,

    NetGroup = 103 //!< Specifies which wires/busses belong to a given net group
};

[[maybe_unused]] static constexpr Structure ToStructure(uint8_t aVal)
{
    return ToEnum<Structure, decltype(aVal)>(aVal);
}

[[maybe_unused]] static std::string to_string(const Structure& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const Structure& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}
} // namespace OOCP
#endif // STRUCTURE_HPP