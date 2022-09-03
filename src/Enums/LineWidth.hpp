#ifndef LINEWIDTH_HPP
#define LINEWIDTH_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"


enum class LineWidth
{
    Thin    = 0,
    Medium  = 1,
    Wide    = 2,
    Default = 3  // Results in a thickness between
                 // Thin and Medium
};


[[maybe_unused]]
static constexpr LineWidth ToLineWidth(uint32_t aVal)
{
    return ToEnum<LineWidth, decltype(aVal)>(aVal);
}


[[maybe_unused]]
static std::string to_string(const LineWidth& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const LineWidth& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // LINEWIDTH_HPP