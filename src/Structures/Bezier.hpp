



#ifndef BEZIER_H
#define BEZIER_H


#include <cstdint>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include "../Structures/Point.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"


struct Bezier
{
    static size_t getExpectedStructSize(FileFormatVersion aVersion, size_t aPointCount);

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

private:

    std::optional<LineStyle> mLineStyle;
    std::optional<LineWidth> mLineWidth;

public:

    std::vector<Point> points;
};


[[maybe_unused]]
static std::string to_string(const Bezier& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "lineStyle  = " + to_string(aObj.getLineStyle()) + newLine();;
    str += indent(1) + "lineWidth  = " + to_string(aObj.getLineWidth()) + newLine();;

    str += indent(1) + "points:" + newLine();
    for(size_t i = 0u; i < aObj.points.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.points[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Bezier& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // BEZIER_H