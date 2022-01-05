#ifndef ROTATION_H
#define ROTATION_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


enum class Rotation
{
    Deg_0   = 0,
    Deg_90  = 1,
    Deg_180 = 2,
    Deg_270 = 3
};


[[maybe_unused]]
static Rotation ToRotation(uint8_t val)
{
    Rotation rotation;

    switch(val)
    {
        case 0: rotation = Rotation::Deg_0;   break;
        case 1: rotation = Rotation::Deg_90;  break;
        case 2: rotation = Rotation::Deg_180; break;
        case 3: rotation = Rotation::Deg_270; break;
        default:
            std::string errorMsg = "Rotation with value " + std::to_string(val)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return rotation;
}


[[maybe_unused]]
static std::string to_string(const Rotation& rotation)
{
    std::string str;

    switch(rotation)
    {
        case Rotation::Deg_0:   str = "0°";   break;
        case Rotation::Deg_90:  str = "90°";  break;
        case Rotation::Deg_180: str = "180°"; break;
        case Rotation::Deg_270: str = "270°"; break;
        default:
            std::string errorMsg = "Rotation " + std::to_string(static_cast<size_t>(rotation))
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const Rotation& rotation)
{
    os << to_string(rotation);
    return os;
}


#endif // ROTATION_H