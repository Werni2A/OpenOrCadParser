#ifndef ROTATION_HPP
#define ROTATION_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"


enum class Rotation
{
    Deg_0   = 0,
    Deg_90  = 1,
    Deg_180 = 2,
    Deg_270 = 3
};


[[maybe_unused]]
static constexpr Rotation ToRotation(uint8_t aVal)
{
    return ToEnum<Rotation, decltype(aVal)>(aVal);
}


[[maybe_unused]]
static std::string to_string(const Rotation& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Rotation& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // ROTATION_HPP