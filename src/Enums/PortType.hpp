#ifndef PORTTYPE_H
#define PORTTYPE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


enum class PortType
{
    Input         = 0,
    Bidirectional = 1,
    Output        = 2,
    OpenCollector = 3,
    Passive       = 4,
    ThreeState    = 5,
    OpenEmitter   = 6,
    Power         = 7
};


[[maybe_unused]]
static PortType ToPortType(uint32_t val)
{
    PortType portType;

    switch(val)
    {
        case 0: portType = PortType::Input;         break;
        case 1: portType = PortType::Bidirectional; break;
        case 2: portType = PortType::Output;        break;
        case 3: portType = PortType::OpenCollector; break;
        case 4: portType = PortType::Passive;       break;
        case 5: portType = PortType::ThreeState;    break;
        case 6: portType = PortType::OpenEmitter;   break;
        case 7: portType = PortType::Power;         break;
        default:
            std::string errorMsg = "PortType with value " + std::to_string(val)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return portType;
}


[[maybe_unused]]
static std::string to_string(const PortType& portType)
{
    std::string str;

    switch(portType)
    {
        case PortType::Input:         str = "Input";         break;
        case PortType::Bidirectional: str = "Bidirectional"; break;
        case PortType::Output:        str = "Output";        break;
        case PortType::OpenCollector: str = "OpenCollector"; break;
        case PortType::Passive:       str = "Passive";       break;
        case PortType::ThreeState:    str = "ThreeState";    break;
        case PortType::OpenEmitter:   str = "OpenEmitter";   break;
        case PortType::Power:         str = "Power";         break;
        default:
            std::string errorMsg = "PortType " + std::to_string(static_cast<size_t>(portType))
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const PortType& portType)
{
    os << to_string(portType);
    return os;
}


#endif // PORTTYPE_H