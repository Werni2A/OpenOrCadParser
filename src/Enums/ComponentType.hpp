#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H


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
#include <ctime>
#include <utility>
#include <vector>

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
static ComponentType ToComponentType(uint16_t val)
{
    ComponentType componentType;

    switch(val)
    {
        case 0x06: componentType = ComponentType::Cell;             break;
        case 0x09: componentType = ComponentType::View;             break;
        case 0x18: componentType = ComponentType::Part;             break;
        case 0x1f: componentType = ComponentType::Package;          break;
        case 0x21: componentType = ComponentType::GlobalSymbol;     break;
        case 0x22: componentType = ComponentType::PortSymbol;       break;
        case 0x23: componentType = ComponentType::OffPageSymbol;    break;
        case 0x40: componentType = ComponentType::TitleBlockSymbol; break;
        case 0x4b: componentType = ComponentType::ERCSymbol;        break;
        case 0x62: componentType = ComponentType::PinShapeSymbol;   break;
        case 0x2e80: componentType = ComponentType::CT0x2e80;       break;
        case 0x444e: componentType = ComponentType::CT0x444e;       break;
        default:
            std::string errorMsg = "ComponentType with value 0x" + ToHex(val, 2)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return componentType;
}


[[maybe_unused]]
static std::string to_string(const ComponentType& componentType)
{
    std::string str;

    switch(componentType)
    {
        case ComponentType::Cell:             str = "Cell";             break;
        case ComponentType::View:             str = "View";             break;
        case ComponentType::Part:             str = "Part";             break;
        case ComponentType::Package:          str = "Package";          break;
        case ComponentType::GlobalSymbol:     str = "GlobalSymbol";     break;
        case ComponentType::PortSymbol:       str = "PortSymbol";       break;
        case ComponentType::OffPageSymbol:    str = "OffPageSymbol";    break;
        case ComponentType::TitleBlockSymbol: str = "TitleBlockSymbol"; break;
        case ComponentType::ERCSymbol:        str = "ERCSymbol";        break;
        case ComponentType::PinShapeSymbol:   str = "PinShapeSymbol";   break;
        case ComponentType::CT0x2e80:         str = "CT0x2e80";         break;
        case ComponentType::CT0x444e:         str = "CT0x444e";         break;
        default:
            std::string errorMsg = "ComponentType 0x" + ToHex(componentType, 2)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const ComponentType& componentType)
{
    os << to_string(componentType);
    return os;
}


// @todo Type has nothing to do with ComponentType, therefore move it to its own file.
//       It's used for $Type$ files
struct Type
{
    std::string name;
    ComponentType componentType;
};


[[maybe_unused]]
static std::string to_string(const Type& type)
{
    std::string str;

    str += "Type:" + newLine();
    str += "  name = " + type.name + newLine();
    str += "  componentType = " + to_string(type.componentType) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Type& type)
{
    os << to_string(type);

    return os;
}


#endif // COMPONENTTYPE_H