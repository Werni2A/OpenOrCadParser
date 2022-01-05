#ifndef COLOR_H
#define COLOR_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


enum class Color
{
    VeryLightRed            =  0, // Color code: #ff8080
    VeryLightYellow         =  1, // Color code: #ffff80
    VeryLightLimeGreen      =  2, // Color code: #80ff80
    Cyan2                   =  3, // Color code: #00ff80
    VeryLightCyan           =  4, // Color code: #80ffff
    Blue2                   =  5, // Color code: #0080ff
    VeryLightPink           =  6, // Color code: #ff80c0
    VeryLightMagenta        =  7, // Color code: #ff80ff
    Red                     =  8, // Color code: #ff0000
    Yellow                  =  9, // Color code: #ffff00
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


[[maybe_unused]]
static Color ToColor(uint32_t val)
{
    Color color;

    switch(val)
    {
        case  0: color = Color::VeryLightRed;            break;
        case  1: color = Color::VeryLightYellow;         break;
        case  2: color = Color::VeryLightLimeGreen;      break;
        case  3: color = Color::Cyan2;                   break;
        case  4: color = Color::VeryLightCyan;           break;
        case  5: color = Color::Blue2;                   break;
        case  6: color = Color::VeryLightPink;           break;
        case  7: color = Color::VeryLightMagenta;        break;
        case  8: color = Color::Red;                     break;
        case  9: color = Color::Yellow;                  break;
        case 10: color = Color::Green;                   break;
        case 11: color = Color::Cyan1;                   break;
        case 12: color = Color::Cyan3;                   break;
        case 13: color = Color::StrongBlue;              break;
        case 14: color = Color::SlightlyDesaturatedBlue; break;
        case 15: color = Color::Magenta;                 break;
        case 16: color = Color::DarkModerateRed;         break;
        case 17: color = Color::LightOrange;             break;
        case 18: color = Color::LimeGreen2;              break;
        case 19: color = Color::DarkCyan;                break;
        case 20: color = Color::DarkBlue2;               break;
        case 21: color = Color::VeryLightBlue;           break;
        case 22: color = Color::DarkPink;                break;
        case 23: color = Color::Pink;                    break;
        case 24: color = Color::DarkRed;                 break;
        case 25: color = Color::Orange;                  break;
        case 26: color = Color::DarkLimeGreen;           break;
        case 27: color = Color::LimeGreen1;              break;
        case 28: color = Color::Blue1;                   break;
        case 29: color = Color::DarkBlue1;               break;
        case 30: color = Color::DarkMagenta;             break;
        case 31: color = Color::Violet;                  break;
        case 32: color = Color::VeryDarkRed;             break;
        case 33: color = Color::DarkOrange;              break;
        case 34: color = Color::VeryDarkLimeGreen;       break;
        case 35: color = Color::VeryDarkCyan;            break;
        case 36: color = Color::VeryDarkBlue2;           break;
        case 37: color = Color::VeryDarkBlue1;           break;
        case 38: color = Color::VeryDarkDagenta1;        break;
        case 39: color = Color::DarkViolet;              break;
        case 40: color = Color::Black;                   break;
        case 41: color = Color::DarkYellow;              break;
        case 42: color = Color::DarkModerateYellow;      break;
        case 43: color = Color::DarkGray;                break;
        case 44: color = Color::DarkModerateCyan;        break;
        case 45: color = Color::LightGray;               break;
        case 46: color = Color::VeryDarkMagenta2;        break;
        case 47: color = Color::White;                   break;
        case 48: color = Color::Default;                 break;
        default:
            std::string errorMsg = "Color with value " + std::to_string(val)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return color;
}


[[maybe_unused]]
static std::string to_string(const Color& color)
{
    std::string str;

    switch(color)
    {
        case Color::VeryLightRed:            str = "VeryLightRed";            break;
        case Color::VeryLightYellow:         str = "VeryLightYellow";         break;
        case Color::VeryLightLimeGreen:      str = "VeryLightLimeGreen";      break;
        case Color::Cyan2:                   str = "Cyan2";                   break;
        case Color::VeryLightCyan:           str = "VeryLightCyan";           break;
        case Color::Blue2:                   str = "Blue2";                   break;
        case Color::VeryLightPink:           str = "VeryLightPink";           break;
        case Color::VeryLightMagenta:        str = "VeryLightMagenta";        break;
        case Color::Red:                     str = "Red";                     break;
        case Color::Yellow:                  str = "Yellow";                  break;
        case Color::Green:                   str = "Green";                   break;
        case Color::Cyan1:                   str = "Cyan1";                   break;
        case Color::Cyan3:                   str = "Cyan3";                   break;
        case Color::StrongBlue:              str = "StrongBlue";              break;
        case Color::SlightlyDesaturatedBlue: str = "SlightlyDesaturatedBlue"; break;
        case Color::Magenta:                 str = "Magenta";                 break;
        case Color::DarkModerateRed:         str = "DarkModerateRed";         break;
        case Color::LightOrange:             str = "LightOrange";             break;
        case Color::LimeGreen2:              str = "LimeGreen2";              break;
        case Color::DarkCyan:                str = "DarkCyan";                break;
        case Color::DarkBlue2:               str = "DarkBlue2";               break;
        case Color::VeryLightBlue:           str = "VeryLightBlue";           break;
        case Color::DarkPink:                str = "DarkPink";                break;
        case Color::Pink:                    str = "Pink";                    break;
        case Color::DarkRed:                 str = "DarkRed";                 break;
        case Color::Orange:                  str = "Orange";                  break;
        case Color::DarkLimeGreen:           str = "DarkLimeGreen";           break;
        case Color::LimeGreen1:              str = "LimeGreen1";              break;
        case Color::Blue1:                   str = "Blue1";                   break;
        case Color::DarkBlue1:               str = "DarkBlue1";               break;
        case Color::DarkMagenta:             str = "DarkMagenta";             break;
        case Color::Violet:                  str = "Violet";                  break;
        case Color::VeryDarkRed:             str = "VeryDarkRed";             break;
        case Color::DarkOrange:              str = "DarkOrange";              break;
        case Color::VeryDarkLimeGreen:       str = "VeryDarkLimeGreen";       break;
        case Color::VeryDarkCyan:            str = "VeryDarkCyan";            break;
        case Color::VeryDarkBlue2:           str = "VeryDarkBlue2";           break;
        case Color::VeryDarkBlue1:           str = "VeryDarkBlue1";           break;
        case Color::VeryDarkDagenta1:        str = "VeryDarkDagenta1";        break;
        case Color::DarkViolet:              str = "DarkViolet";              break;
        case Color::Black:                   str = "Black";                   break;
        case Color::DarkYellow:              str = "DarkYellow";              break;
        case Color::DarkModerateYellow:      str = "DarkModerateYellow";      break;
        case Color::DarkGray:                str = "DarkGray";                break;
        case Color::DarkModerateCyan:        str = "DarkModerateCyan";        break;
        case Color::LightGray:               str = "LightGray";               break;
        case Color::VeryDarkMagenta2:        str = "VeryDarkMagenta2";        break;
        case Color::White:                   str = "White";                   break;
        case Color::Default:                 str = "Default";                 break;
        default:
            std::string errorMsg = "Color " + std::to_string(static_cast<size_t>(color))
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Color& color)
{
    os << to_string(color);
    return os;
}


#endif // COLOR_H