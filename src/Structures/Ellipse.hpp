



#ifndef ELLIPSE_H
#define ELLIPSE_H


#include <cstdint>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>

#include "../Enums/HatchStyle.hpp"
#include "../Enums/FillStyle.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"


struct Ellipse
{
    static size_t getExpectedStructSize(FileFormatVersion aVersion);

    void setLineStyle(const LineStyle& aVal)
    {
        mLineStyle = std::make_optional<LineStyle>(aVal);
    }

    LineStyle getLineStyle() const
    {
        return mLineStyle.value_or(LineStyle::Solid);
    }

    void setLineWidth(const LineWidth& aVal)
    {
        mLineWidth = std::make_optional<LineWidth>(aVal);
    }

    LineWidth getLineWidth() const
    {
        return mLineWidth.value_or(LineWidth::Default);
    }

    void setFillStyle(const FillStyle& aVal)
    {
        mFillStyle = std::make_optional<FillStyle>(aVal);
    }

    FillStyle getFillStyle() const
    {
        return mFillStyle.value_or(FillStyle::None);
    }

    void setHatchStyle(const HatchStyle& aVal)
    {
        mHatchStyle = std::make_optional<HatchStyle>(aVal);
    }

    HatchStyle getHatchStyle() const
    {
        return mHatchStyle.value_or(HatchStyle::NotValid);
    }

    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;

private:

    std::optional<LineStyle>  mLineStyle;
    std::optional<LineWidth>  mLineWidth;

    std::optional<FillStyle>  mFillStyle;
    std::optional<HatchStyle> mHatchStyle;
};


[[maybe_unused]]
static std::string to_string(const Ellipse& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "x1 = " + std::to_string(aObj.x1) + newLine();
    str += indent(1) + "y1 = " + std::to_string(aObj.y1) + newLine();
    str += indent(1) + "x2 = " + std::to_string(aObj.x2) + newLine();
    str += indent(1) + "y2 = " + std::to_string(aObj.y2) + newLine();
    str += indent(1) + "lineStyle  = " + to_string(aObj.getLineStyle()) + newLine();
    str += indent(1) + "lineWidth  = " + to_string(aObj.getLineWidth()) + newLine();
    str += indent(1) + "fillStyle  = " + to_string(aObj.getFillStyle()) + newLine();
    str += indent(1) + "hatchStyle = " + to_string(aObj.getHatchStyle()) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Ellipse& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // ELLIPSE_H