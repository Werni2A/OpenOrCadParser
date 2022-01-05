#ifndef PINSHAPE_H
#define PINSHAPE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "General.hpp"


// @todo this file is highly experimental and needs further testing

struct PinShape // @todo rename to something like PinProperties as it contains more than the shape
{
    // @todo Move comments into Kaitai file
    bool isLong;  // ---  // Is always 0. Probably the bit position is wrong.
    bool isClock; // |>
    bool isDot;   // o|
    bool isLeftPointing;
    bool isRightPointing;  // @note this can be derived from isLeftPointing... is it really in the file? or is
                           // there the possibility to point in both directions as there is to point in none?
    bool isNetStyle;
    bool isNoConnect;
    bool isGlobal;
    bool isNumberVisible;
};


enum class ShapeType
{
    Clock,
    Dot,
    DotClock,
    Line,
    Short,
    ShortClock,
    ShortDot,
    ShortDotClock,
    ZeroLength
};


// @todo Move table to Kaitai file
// See OrCAD: 'Pin Properties' -> 'Shape'
// 'Clock'             <-  isLong &&  isClock && !isDot
// 'Dot'               <-  isLong && !isClock &&  isDot
// 'Dot-Clock'         <-  isLong &&  isClock &&  isDot
// 'Line'              <-  isLong && !isClock && !isDot
// 'Short'             <- !isLong && !isClock && !isDot
// 'Short Clock'       <- !isLong &&  isClock && !isDot
// 'Short Dot'         <- !isLong && !isClock &&  isDot
// 'Short Dot Clock'   <- !isLong &&  isClock &&  isDot
// 'Zero Length'       // Not sure how this is generated
[[maybe_unused]]
static ShapeType ToShapeType(const PinShape& pinShape)
{
    const auto& pin = pinShape;

    ShapeType shapeType;

    if( pin.isLong &&  pin.isClock && !pin.isDot)
    {
        shapeType = ShapeType::Clock;
    }
    if( pin.isLong && !pin.isClock &&  pin.isDot)
    {
        shapeType = ShapeType::Dot;
    }
    if( pin.isLong &&  pin.isClock &&  pin.isDot)
    {
        shapeType = ShapeType::DotClock;
    }
    if( pin.isLong && !pin.isClock && !pin.isDot)
    {
        shapeType = ShapeType::Line;
    }
    if(!pin.isLong && !pin.isClock && !pin.isDot)
    {
        shapeType = ShapeType::Short;
    }
    if(!pin.isLong &&  pin.isClock && !pin.isDot)
    {
        shapeType = ShapeType::ShortClock;
    }
    if(!pin.isLong && !pin.isClock &&  pin.isDot)
    {
        shapeType = ShapeType::ShortDot;
    }
    if(!pin.isLong &&  pin.isClock &&  pin.isDot)
    {
        shapeType = ShapeType::ShortDotClock;
    }
    if(false) // @todo Not sure how this is generated
    {
        shapeType = ShapeType::ZeroLength;
    }
    else
    {
        throw std::runtime_error("Strange combination of PinShape that is unknown!");
    }


    return shapeType;
}


[[maybe_unused]]
static std::string to_string(const ShapeType& shapeType)
{
    std::string str;

    switch(shapeType)
    {
        case ShapeType::Clock:         str = "Clock";         break;
        case ShapeType::Dot:           str = "Dot";           break;
        case ShapeType::DotClock:      str = "DotClock";      break;
        case ShapeType::Line:          str = "Line";          break;
        case ShapeType::Short:         str = "Short";         break;
        case ShapeType::ShortClock:    str = "ShortClock";    break;
        case ShapeType::ShortDot:      str = "ShortDot";      break;
        case ShapeType::ShortDotClock: str = "ShortDotClock"; break;
        case ShapeType::ZeroLength:    str = "ZeroLength";    break;
        default:
            std::string errorMsg = "ShapeType " + std::to_string(static_cast<size_t>(shapeType))
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const ShapeType& shapeType)
{
    os << to_string(shapeType) << std::endl;
    return os;
}


[[maybe_unused]]
static PinShape ToPinShape(uint16_t val)
{
    PinShape pinShape;

    pinShape.isLong          = static_cast<bool>(GetBit(0, val)); // @todo does not work. Probably at different position
    pinShape.isClock         = static_cast<bool>(GetBit(1, val));
    pinShape.isDot           = static_cast<bool>(GetBit(2, val));
    pinShape.isLeftPointing  = static_cast<bool>(GetBit(3, val)); // @todo needs verification
    pinShape.isRightPointing = static_cast<bool>(GetBit(4, val)); // @todo needs verification
    pinShape.isGlobal        = static_cast<bool>(GetBit(5, val)); // @todo needs verification
    pinShape.isNetStyle      = static_cast<bool>(GetBit(6, val)); // @todo needs verification
    pinShape.isNoConnect     = static_cast<bool>(GetBit(7, val)); // @todo needs verification
    // pinShape.Unknown         = static_cast<bool>(GetBit(10, val)); // @todo needs verification

    // pinShape.isNumberVisible = static_cast<bool>(GetBit(8, val)); // @todo is covered by generalPropertie

    for(size_t i = 8u; i < sizeof(val) * 8u; ++i)
    {
        // @todo What does bit 10 represent?
        if(i == 10u)
        {
            continue;
        }

        if(GetBit(i, val) == 1u)
        {
            throw std::runtime_error("PinShape bit " + std::to_string(i) + " is set but not known!");
        }
    }

    return pinShape;
}


[[maybe_unused]]
static std::string to_string(const PinShape& pinShape)
{
    std::string str;

    str += "PinShape:" + newLine();
    str += indent(1) + "isLong          = " + std::to_string(pinShape.isLong)          + newLine();
    str += indent(1) + "isClock         = " + std::to_string(pinShape.isClock)         + newLine();
    str += indent(1) + "isDot           = " + std::to_string(pinShape.isDot)           + newLine();
    str += indent(1) + "isLeftPointing  = " + std::to_string(pinShape.isLeftPointing)  + newLine();
    str += indent(1) + "isRightPointing = " + std::to_string(pinShape.isRightPointing) + newLine();
    str += indent(1) + "isNetStyle      = " + std::to_string(pinShape.isNetStyle)      + newLine();
    str += indent(1) + "isNoConnect     = " + std::to_string(pinShape.isNoConnect)     + newLine();
    str += indent(1) + "isGlobal        = " + std::to_string(pinShape.isGlobal)        + newLine();
    str += indent(1) + "isNumberVisible = " + std::to_string(pinShape.isNumberVisible) + newLine();

    // @todo Deactivated until isLong is handled correctly
    // str += indent(1) + "ShapeType       = " + ToShapeType(pinShape) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const PinShape& pinShape)
{
    os << to_string(pinShape);
    return os;
}


#endif // PINSHAPE_H