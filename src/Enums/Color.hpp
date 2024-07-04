#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"

enum class Color
{
    VeryLightRed            = 0,  // Color code: #ff8080
    VeryLightYellow         = 1,  // Color code: #ffff80
    VeryLightLimeGreen      = 2,  // Color code: #80ff80
    Cyan2                   = 3,  // Color code: #00ff80
    VeryLightCyan           = 4,  // Color code: #80ffff
    Blue2                   = 5,  // Color code: #0080ff
    VeryLightPink           = 6,  // Color code: #ff80c0
    VeryLightMagenta        = 7,  // Color code: #ff80ff
    Red                     = 8,  // Color code: #ff0000
    Yellow                  = 9,  // Color code: #ffff00
    Green                   = 10, // Color code: #80ff00
    Cyan1                   = 11, // Color code: #00ff40
    Cyan3                   = 12, // Color code: #00ffff
    StrongBlue              = 13, // Color code: #0080c0
    SlightlyDesaturatedBlue = 14, // Color code: #8080c0
    Magenta                 = 15, // Color code: #ff00ff
    DarkModerateRed         = 16, // Color code: #804040
    LightOrange             = 17, // Color code: #ff8040
    LimeGreen2              = 18, // Color code: #00ff00
    DarkCyan                = 19, // Color code: #008080
    DarkBlue2               = 20, // Color code: #004080
    VeryLightBlue           = 21, // Color code: #8080ff
    DarkPink                = 22, // Color code: #800040
    Pink                    = 23, // Color code: #ff0080
    DarkRed                 = 24, // Color code: #800000
    Orange                  = 25, // Color code: #ff8000
    DarkLimeGreen           = 26, // Color code: #008000
    LimeGreen1              = 27, // Color code: #008040
    Blue1                   = 28, // Color code: #0000ff
    DarkBlue1               = 29, // Color code: #0000a0
    DarkMagenta             = 30, // Color code: #800080
    Violet                  = 31, // Color code: #8000ff
    VeryDarkRed             = 32, // Color code: #400000
    DarkOrange              = 33, // Color code: #804000
    VeryDarkLimeGreen       = 34, // Color code: #004000
    VeryDarkCyan            = 35, // Color code: #004040
    VeryDarkBlue2           = 36, // Color code: #000080
    VeryDarkBlue1           = 37, // Color code: #000040
    VeryDarkDagenta1        = 38, // Color code: #400040
    DarkViolet              = 39, // Color code: #400080
    Black                   = 40, // Color code: #000000
    DarkYellow              = 41, // Color code: #808000
    DarkModerateYellow      = 42, // Color code: #808040
    DarkGray                = 43, // Color code: #808080
    DarkModerateCyan        = 44, // Color code: #408080
    LightGray               = 45, // Color code: #c0c0c0
    VeryDarkMagenta2        = 46, // Color code: #400040
    White                   = 47, // Color code: #ffffff
    Default                 = 48
};

[[maybe_unused]] static constexpr Color ToColor(uint32_t aVal)
{
    return ToEnum<Color, decltype(aVal)>(aVal);
}

[[maybe_unused]] static std::string to_string(const Color& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const Color& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}

#endif // COLOR_HPP