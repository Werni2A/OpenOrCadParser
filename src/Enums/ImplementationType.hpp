#ifndef IMPLEMENTATIONTYPE_HPP
#define IMPLEMENTATIONTYPE_HPP

#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"

namespace OOCP
{
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
static constexpr ImplementationType ToImplementationType(uint8_t aVal)
{
    return ToEnum<ImplementationType, decltype(aVal)>(aVal);
}

[[maybe_unused]]
static std::string to_string(const ImplementationType& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const ImplementationType& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}
} // namespace OOCP
#endif // IMPLEMENTATIONTYPE_HPP