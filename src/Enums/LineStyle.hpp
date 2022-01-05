#ifndef LINESTYLE_H
#define LINESTYLE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


// @todo this should be a better solution
struct LineStyle2
{
    enum class Value
    {
        Solid = 0,
        Foo   = 42
    };

private:
    Value mVal;

public:
    LineStyle2() = delete;

    LineStyle2(uint32_t aVal)
    {
        switch(aVal)
        {
            case 0: mVal = LineStyle2::Value::Solid; break;
        }
    }

    constexpr LineStyle2::Value get() const
    {
        return mVal;
    }

    constexpr std::string to_string() const;

};



enum class LineStyle
{
    Solid      = 0, // ______
    Dash       = 1, // ------
    Dot        = 2, // ......
    DashDot    = 3, // -.-.-.
    DashDotDot = 4, // -..-..
    Default    = 5
};


[[maybe_unused]]
static LineStyle ToLineStyle(uint32_t val)
{
    LineStyle lineStyle;

    switch(val)
    {
        case 0: lineStyle = LineStyle::Solid;      break;
        case 1: lineStyle = LineStyle::Dash;       break;
        case 2: lineStyle = LineStyle::Dot;        break;
        case 3: lineStyle = LineStyle::DashDot;    break;
        case 4: lineStyle = LineStyle::DashDotDot; break;
        case 5: lineStyle = LineStyle::Default;    break;
        default:
            std::string errorMsg = "LineStyle with value " + std::to_string(val)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return lineStyle;
}


[[maybe_unused]]
static std::string to_string(const LineStyle& lineStyle)
{
    std::string str;

    switch(lineStyle)
    {
        case LineStyle::Solid:      str = "Solid";      break;
        case LineStyle::Dash:       str = "Dash";       break;
        case LineStyle::Dot:        str = "Dot";        break;
        case LineStyle::DashDot:    str = "DashDot";    break;
        case LineStyle::DashDotDot: str = "DashDotDot"; break;
        case LineStyle::Default:    str = "Default";    break;
        default:
            std::string errorMsg = "LineStyle " + std::to_string(static_cast<size_t>(lineStyle))
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const LineStyle& lineStyle)
{
    os << to_string(lineStyle);
    return os;
}


#endif // LINESTYLE_H