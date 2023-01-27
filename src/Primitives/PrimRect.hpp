#ifndef PRIMRECT_HPP
#define PRIMRECT_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/FillStyle.hpp"
#include "Enums/HatchStyle.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Primitives/Point.hpp"
#include "Primitives/PrimBase.hpp"


class PrimRect : public PrimBase
{
public:
    PrimRect(DataStream& aDs) : PrimBase{aDs}, x1{0}, y1{0}, x2{0}, y2{0},
        mLineStyle{}, mLineWidth{}, fillStyle{}, hatchStyle{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

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
static std::string to_string(const PrimRect& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}x1 = {}\n", indent(1), aObj.x1);
    str += fmt::format("{}y1 = {}\n", indent(1), aObj.y1);
    str += fmt::format("{}x2 = {}\n", indent(1), aObj.x2);
    str += fmt::format("{}y2 = {}\n", indent(1), aObj.y2);
    str += fmt::format("{}lineStyle  = {}\n", indent(1), to_string(aObj.getLineStyle()));
    str += fmt::format("{}lineWidth  = {}\n", indent(1), to_string(aObj.getLineWidth()));
    str += fmt::format("{}fillStyle  = {}\n", indent(1), to_string(aObj.fillStyle));
    str += fmt::format("{}hatchStyle = {}\n", indent(1), to_string(aObj.hatchStyle));

    return str;
}


inline std::string PrimRect::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PrimRect& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PRIMRECT_HPP