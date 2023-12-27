#ifndef COORDINATE_HPP
#define COORDINATE_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "PrimBase.hpp"


/// @note Represents 'BezierPoint', 'PolygonPoint' and 'PolylinePoint' because
///       all of them use the same point structure.
class Point : public PrimBase
{
public:

    Point(StreamContext& aCtx) : PrimBase{aCtx}, x{0}, y{0}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    uint16_t x;
    uint16_t y;
};


[[maybe_unused]]
static std::string to_string(const Point& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}x = {}\n", indent(1), aObj.x);
    str += fmt::format("{}y = {}\n", indent(1), aObj.y);
    return str;
}


inline std::string Point::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Point& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // COORDINATE_HPP