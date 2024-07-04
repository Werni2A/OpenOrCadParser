#ifndef PRIMPOLYGON_HPP
#define PRIMPOLYGON_HPP

#include <cstdint>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/FillStyle.hpp"
#include "Enums/HatchStyle.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Primitives/Point.hpp"
#include "Primitives/PrimBase.hpp"

class PrimPolygon : public PrimBase
{
public:
    PrimPolygon(StreamContext& aCtx)
        : PrimBase{aCtx},
          mLineStyle{},
          mLineWidth{},
          fillStyle{},
          hatchStyle{},
          points{}
    {
    }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    Primitive getObjectType() const override
    {
        return Primitive::Polygon;
    }

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
};

[[maybe_unused]] static std::string to_string(const PrimPolygon& aObj)
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

inline std::string PrimPolygon::to_string() const
{
    return ::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const PrimPolygon& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}

#endif // PRIMPOLYGON_HPP