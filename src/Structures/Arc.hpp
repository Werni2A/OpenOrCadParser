#ifndef ARC_H
#define ARC_H


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"


// @note This is additionally defines an elliptic arc.
struct Arc
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

    int32_t startX;
    int32_t startY;

    int32_t endX;
    int32_t endY;

private:

    std::optional<LineStyle> mLineStyle;
    std::optional<LineWidth> mLineWidth;
};


[[maybe_unused]]
static std::string to_string(const Arc& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "x1 = " + std::to_string(aObj.x1) + newLine();
    str += indent(1) + "y1 = " + std::to_string(aObj.y1) + newLine();
    str += indent(1) + "x2 = " + std::to_string(aObj.x2) + newLine();
    str += indent(1) + "y2 = " + std::to_string(aObj.y2) + newLine();
    str += indent(1) + "startX = " + std::to_string(aObj.startX)  + newLine();
    str += indent(1) + "startY = " + std::to_string(aObj.startY)  + newLine();
    str += indent(1) + "endX   = " + std::to_string(aObj.endX)    + newLine();
    str += indent(1) + "endY   = " + std::to_string(aObj.endY)    + newLine();
    str += indent(1) + "lineStyle = " + to_string(aObj.getLineStyle()) + newLine();
    str += indent(1) + "lineWidth = " + to_string(aObj.getLineWidth()) + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Arc& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // ARC_H