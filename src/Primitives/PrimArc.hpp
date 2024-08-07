#ifndef PRIMARC_HPP
#define PRIMARC_HPP

#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Primitives/PrimBase.hpp"

namespace OOCP
{
// @note This defines an elliptic arc.
class PrimArc : public PrimBase
{
public:
    PrimArc(StreamContext& aCtx)
        : PrimBase{aCtx},
          x1{0},
          y1{0},
          x2{0},
          y2{0},
          startX{0},
          startY{0},
          endX{0},
          endY{0},
          mLineStyle{},
          mLineWidth{}
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
        return Primitive::Arc;
    }

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

    // Elliptic Arc
    // See:
    // PlaceArc: http://www.wictronic.ch/Downloads/PSpice/Version_10_5/manuals/Capture_Users_Guide_10_5.pdf#page=316
    // http://geocities.ws/web_sketches/ellipse_notes/ellipse_arc_length/ellipse_arc_length.html

    // Vertex 1 and Vertex 2 span up a rectangle that limits the ellipse.
    // I.e. width and height of the rectangle define the two radiuses of
    // the ellipse. Note that a vertex plus the radiuses gives the center
    // of the ellipse (position where its placed).

    // Vertex 1
    int32_t x1;
    int32_t y1;

    // Vertex 2
    int32_t x2;
    int32_t y2;

    // Start point of the arc
    int32_t startX;
    int32_t startY;

    // End point of the arc
    int32_t endX;
    int32_t endY;

private:
    std::optional<LineStyle> mLineStyle;
    std::optional<LineWidth> mLineWidth;
};

[[maybe_unused]]
static std::string to_string(const PrimArc& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("x1     = {}\n", aObj.x1);
    str += fmt::format("y1     = {}\n", aObj.y1);
    str += fmt::format("x2     = {}\n", aObj.x2);
    str += fmt::format("y2     = {}\n", aObj.y2);
    str += fmt::format("startX = {}\n", aObj.startX);
    str += fmt::format("startY = {}\n", aObj.startY);
    str += fmt::format("endX   = {}\n", aObj.endX);
    str += fmt::format("endY   = {}\n", aObj.endY);
    str += fmt::format("lineStyle = {}\n", to_string(aObj.getLineStyle()));
    str += fmt::format("lineWidth = {}\n", to_string(aObj.getLineWidth()));

    return str;
}

inline std::string PrimArc::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PrimArc& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP
#endif // PRIMARC_HPP