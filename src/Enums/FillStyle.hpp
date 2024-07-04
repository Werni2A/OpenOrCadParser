#ifndef FILLSTYLE_HPP
#define FILLSTYLE_HPP

#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"

// This configuration dominates the hatch_style.
// I.e. when solid/none is set, the hatch_style
// value is ignored.
enum class FillStyle
{
    Solid        = 0,
    None         = 1,
    HatchPattern = 2 // Refer to HatchStyle
};

[[maybe_unused]] static constexpr FillStyle ToFillStyle(uint32_t aVal)
{
    return ToEnum<FillStyle, decltype(aVal)>(aVal);
}

[[maybe_unused]] static std::string to_string(const FillStyle& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const FillStyle& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}

#endif // FILLSTYLE_HPP