#ifndef POLYGON_H
#define POLYGON_H


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../Structures/Point.hpp"
#include "../Enums/FillStyle.hpp"
#include "../Enums/HatchStyle.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"


struct Polygon
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

    FillStyle fillStyle;
    HatchStyle hatchStyle;

    std::vector<Point> points;

    std::string to_string() const;
};


// @todo this looks like a better solution for to_string. Implement for all other
//       structures and remove operator<<.
// std::string Polygon::to_string() const
// {
//     std::string str;

//     str += "Polygon:" + newLine();
//     str += indent(1) + "lineStyle  = " + ::to_string(lineStyle) + newLine();
//     str += indent(1) + "lineWidth  = " + ::to_string(lineWidth) + newLine();
//     str += indent(1) + "fillStyle  = " + ::to_string(fillStyle) + newLine();

//     if(fillStyle == FillStyle::HatchPattern)
//     {
//         str += indent(1) + "hatchStyle = " + ::to_string(hatchStyle) + newLine();
//     }

//     str += fmt::format("{}points:\n", indent(1));
//     for(size_t i = 0u; i < points.size(); ++i)
//     {
//         str += indent(fmt::format("{}: {}", i, to_string(points[i])), 2);
//     }

//     return str;
// }


[[maybe_unused]]
static std::string to_string(const Polygon& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}lineStyle  = {}\n", indent(1), to_string(aObj.getLineStyle()));
    str += fmt::format("{}lineWidth  = {}\n", indent(1), to_string(aObj.getLineWidth()));
    str += fmt::format("{}fillStyle  = {}\n", indent(1), to_string(aObj.fillStyle));
    str += fmt::format("{}hatchStyle = {}\n", indent(1), to_string(aObj.hatchStyle));

    str += fmt::format("{}points:\n", indent(1));
    for(size_t i = 0u; i < aObj.points.size(); ++i)
    {
        str += indent(fmt::format("{}: {}", i, to_string(aObj.points[i])), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Polygon& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // POLYGON_H