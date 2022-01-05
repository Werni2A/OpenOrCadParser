#ifndef IMPLEMENTATIONTYPE_H
#define IMPLEMENTATIONTYPE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


enum class ImplementationType
{
    None           = 0,
    SchematicView  = 1,
    VHDL           = 2,
    EDIF           = 3,
    Project        = 4,
    PSpiceModel    = 5,
    PSpiceStimulus = 6,
    Verilog        = 7
};


[[maybe_unused]]
static ImplementationType ToImplementationType(uint8_t val)
{
    ImplementationType implementationType;

    switch(val)
    {
        case 0: implementationType = ImplementationType::None;           break;
        case 1: implementationType = ImplementationType::SchematicView;  break;
        case 2: implementationType = ImplementationType::VHDL;           break;
        case 3: implementationType = ImplementationType::EDIF;           break;
        case 4: implementationType = ImplementationType::Project;        break;
        case 5: implementationType = ImplementationType::PSpiceModel;    break;
        case 6: implementationType = ImplementationType::PSpiceStimulus; break;
        case 7: implementationType = ImplementationType::Verilog;        break;
        default:
            std::string errorMsg = "ImplementationType with value " + std::to_string(val)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return implementationType;
}


[[maybe_unused]]
static std::string to_string(const ImplementationType& implementationType)
{
    std::string str;

    switch(implementationType)
    {
        case ImplementationType::None:           str = "None";           break;
        case ImplementationType::SchematicView:  str = "SchematicView";  break;
        case ImplementationType::VHDL:           str = "VHDL";           break;
        case ImplementationType::EDIF:           str = "EDIF";           break;
        case ImplementationType::Project:        str = "Project";        break;
        case ImplementationType::PSpiceModel:    str = "PSpiceModel";    break;
        case ImplementationType::PSpiceStimulus: str = "PSpiceStimulus"; break;
        case ImplementationType::Verilog:        str = "Verilog";        break;
        default:
            std::string errorMsg = "ImplementationType " + std::to_string(static_cast<size_t>(implementationType))
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const ImplementationType& implementationType)
{
    os << to_string(implementationType);
    return os;
}


#endif // IMPLEMENTATIONTYPE_H