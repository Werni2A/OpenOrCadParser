#ifndef PORTTYPE_HPP
#define PORTTYPE_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"


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
static constexpr PortType ToPortType(uint32_t aVal)
{
    return ToEnum<PortType, decltype(aVal)>(aVal);
}


[[maybe_unused]]
static std::string to_string(const PortType& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PortType& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // PORTTYPE_HPP