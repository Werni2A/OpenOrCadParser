#ifndef RECT_H
#define RECT_H


#include <cstdint>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>

#include "../General.hpp"
#include "../Structures/Point.hpp"
#include "../Enums/FillStyle.hpp"
#include "../Enums/HatchStyle.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"


struct Rect
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

    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;

private:

    std::optional<LineStyle> mLineStyle;
    std::optional<LineWidth> mLineWidth;

public:

    FillStyle fillStyle;
    HatchStyle hatchStyle;
};


[[maybe_unused]]
static std::string to_string(const Rect& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "x1 = " + std::to_string(aObj.x1) + newLine();
    str += indent(1) + "y1 = " + std::to_string(aObj.y1) + newLine();
    str += indent(1) + "x2 = " + std::to_string(aObj.x2) + newLine();
    str += indent(1) + "y2 = " + std::to_string(aObj.y2) + newLine();
    str += indent(1) + "lineStyle  = " + to_string(aObj.getLineStyle()) + newLine();
    str += indent(1) + "lineWidth  = " + to_string(aObj.getLineWidth()) + newLine();
    str += indent(1) + "fillStyle  = " + to_string(aObj.fillStyle)  + newLine();
    str += indent(1) + "hatchStyle = " + to_string(aObj.hatchStyle) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Rect& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // RECT_H