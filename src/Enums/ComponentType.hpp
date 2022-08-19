#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H


#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "../General.hpp"


// @todo compare to Structure type. Some of the elements are exactly the same. But it
//       does not seem like we can merge it. Maybe we can split them up into 2 distinct
//       types?
enum class ComponentType
{
    Graphic          =  2,

    Cell             =  6,

    View             =  9,

    Part             = 24,

    Package          = 31,
    PinIdxMapping    = 32, // @todo verify this
    GlobalSymbol     = 33,
    PortSymbol       = 34,
    OffPageSymbol    = 35,

    TitleBlockSymbol = 64,

    ERCSymbol        = 75,
    BookMarkSymbol   = 76,

    PinShapeSymbol   = 98,

    CT_3820          =  3820, // @todo completly unknown and strange looking
    CT_4704          =  4704, // @todo completly unknown and strange looking
    CT_11904         = 11904, // @todo completly unknown and strange looking
    CT_17486         = 17486, // @todo completly unknown and strange looking
    CT_24941         = 24941, // @todo completly unknown and strange looking
    CT_29795         = 29795, // @todo completly unknown and strange looking
    // @todo when this weird types occur also the timezone in directory files is really strange
    //       but not in the XML....? Is this field corrupted? Or somehow coded? But why code
    //       it only sometimes and not always? I think the same values fo the Type also appears
    //       too often to be a realistic corruption issue but I need to confirm this.
};


[[maybe_unused]]
static constexpr ComponentType ToComponentType(uint16_t aVal)
{
    return ToEnum<ComponentType, decltype(aVal)>(aVal);
}


[[maybe_unused]]
static std::string to_string(const ComponentType& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const ComponentType& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // COMPONENTTYPE_H