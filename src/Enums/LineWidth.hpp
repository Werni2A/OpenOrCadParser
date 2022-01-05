#ifndef LINEWIDTH_H
#define LINEWIDTH_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


enum class LineWidth
{
    Thin    = 0,
    Medium  = 1,
    Wide    = 2,
    Default = 3  // Results in a thickness between
                 // Thin and Medium
};


[[maybe_unused]]
static LineWidth ToLineWidth(uint32_t val)
{
    LineWidth lineWidth;

    switch(val)
    {
        case 0: lineWidth = LineWidth::Thin;    break;
        case 1: lineWidth = LineWidth::Medium;  break;
        case 2: lineWidth = LineWidth::Wide;    break;
        case 3: lineWidth = LineWidth::Default; break;
        default:
            std::string errorMsg = "LineWidth with value " + std::to_string(val)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return lineWidth;
}


[[maybe_unused]]
static std::string to_string(const LineWidth& lineWidth)
{
    std::string str;

    switch(lineWidth)
    {
        case LineWidth::Thin:    str = "Thin";    break;
        case LineWidth::Medium:  str = "Medium";  break;
        case LineWidth::Wide:    str = "Wide";    break;
        case LineWidth::Default: str = "Default"; break;
        default:
            std::string errorMsg = "LineWidth " + std::to_string(static_cast<size_t>(lineWidth))
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const LineWidth& lineWidth)
{
    os << to_string(lineWidth);
    return os;
}


#endif // LINEWIDTH_H