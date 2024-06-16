#ifndef PINSHAPE_HPP
#define PINSHAPE_HPP


#include <cstdint>
#include <iostream>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "General.hpp"


// @todo this file is highly experimental and needs further testing

struct PinShape // @todo rename to something like PinProperties as it contains more than the shape
{
    bool isLong;  // ------|  (doubles the pin length iff set)
    bool isClock; //    ---|>
    bool isDot;   //    --o|
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


[[maybe_unused]]
static std::string to_string(const ShapeType& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const ShapeType& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


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

    ShapeType shapeType = ShapeType::Line;

    // All possible combinations are covered
    // by the if-statements

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

    return shapeType;
}


[[maybe_unused]]
static PinShape ToPinShape(uint16_t val, std::optional<spdlog::logger> aLogger = std::nullopt)
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
    // pinShape.isNumberVisible = static_cast<bool>(GetBit(8, val)); // @todo is covered by generalPropertie
    // pinShape.Unknown         = static_cast<bool>(GetBit(10, val)); // @todo needs verification

    for(size_t i = 8u; i < sizeof(val) * 8u; ++i)
    {
        if(GetBit(i, val) == 1u)
        {
            if(i == 8u || i == 9u || i == 10u)
            {
                const std::string msg = fmt::format("{}: PinShape bit {} is set but not known!", __func__, i);

                if(aLogger.has_value())
                {
                    aLogger.value().warn(msg);
                }
                continue;
            }
            else
            {
                const std::string msg = fmt::format("{}: PinShape bit {} is set but 0 was expected!", __func__, i);

                if(aLogger.has_value())
                {
                    aLogger.value().error(msg);
                }
                throw std::runtime_error(msg);
            }
        }
    }

    return pinShape;
}


[[maybe_unused]]
static std::string to_string(const PinShape& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}isLong          = {}\n", indent(1), aObj.isLong);
    str += fmt::format("{}isClock         = {}\n", indent(1), aObj.isClock);
    str += fmt::format("{}isDot           = {}\n", indent(1), aObj.isDot);
    str += fmt::format("{}isLeftPointing  = {}\n", indent(1), aObj.isLeftPointing);
    str += fmt::format("{}isRightPointing = {}\n", indent(1), aObj.isRightPointing);
    str += fmt::format("{}isNetStyle      = {}\n", indent(1), aObj.isNetStyle);
    str += fmt::format("{}isNoConnect     = {}\n", indent(1), aObj.isNoConnect);
    str += fmt::format("{}isGlobal        = {}\n", indent(1), aObj.isGlobal);
    str += fmt::format("{}isNumberVisible = {}\n", indent(1), aObj.isNumberVisible);

    // @todo Deactivated until isLong is handled correctly
    // str += indent(1) + "ShapeType       = " + ToShapeType(aObj) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PinShape& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // PINSHAPE_HPP