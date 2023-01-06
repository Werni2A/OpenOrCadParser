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


// @note This is additionally defines an elliptic arc.
class PrimArc : public PrimBase
{
public:

    PrimArc(DataStream& aDs) : PrimBase{aDs}, x1{0}, y1{0}, x2{0}, y2{0},
        startX{0}, startY{0}, endX{0}, endY{0}, mLineStyle{}, mLineWidth{}
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

    int32_t startX;
    int32_t startY;

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
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PrimArc& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PRIMARC_HPP