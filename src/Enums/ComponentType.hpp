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
    Cell             = 0x06,
    View             = 0x09,
    Part             = 0x18,
    Package          = 0x1f,
    GlobalSymbol     = 0x21,
    PortSymbol       = 0x22,
    OffPageSymbol    = 0x23,
    TitleBlockSymbol = 0x40,
    ERCSymbol        = 0x4b,
    PinShapeSymbol   = 0x62,

    // @todo when this weird types occur also the timezone in directory files is really strange
    //       but not in the XML....? Is this field corrupted? Or somehow coded? But why code
    //       it only sometimes and not always? I think the same values fo the Type also appears
    //       too often to be a realistic corruption issue but I need to confirm this.
    CT0x2e80         = 0x2e80, // @todo completly unknown and strange looking
    CT0x444e         = 0x444e  // @todo completly unknown and strange looking
    // @todo still missing is 'BookMarkSymbol'
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