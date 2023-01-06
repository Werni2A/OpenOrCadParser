#ifndef STRUCTPRIMITIVES_HPP
#define STRUCTPRIMITIVES_HPP


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Primitives/PrimArc.hpp"
#include "Primitives/PrimBezier.hpp"
#include "Primitives/PrimBitmap.hpp"
#include "Primitives/PrimCommentText.hpp"
#include "Primitives/PrimEllipse.hpp"
#include "Primitives/PrimLine.hpp"
#include "Primitives/PrimPolygon.hpp"
#include "Primitives/PrimPolyline.hpp"
#include "Primitives/PrimRect.hpp"
#include "Primitives/PrimSymbolVector.hpp"


class StructPrimitives : public CommonBase
{
public:

    StructPrimitives(DataStream& aDs) : CommonBase{aDs}, name{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    FileFormatVersion predictVersion();

    std::string name;

    // @todo replace all this stuff with PrimBase vector
    std::vector<PrimRect>         rects;
    std::vector<PrimLine>         lines;
    std::vector<PrimArc>          arcs;
    std::vector<PrimEllipse>      ellipses;
    std::vector<PrimPolygon>      polygons;
    std::vector<PrimPolyline>     polylines;
    std::vector<PrimCommentText>  commentTexts;
    std::vector<PrimBitmap>       bitmaps;
    std::vector<PrimSymbolVector> symbolVectors;
    std::vector<PrimBezier>       beziers;
};


[[maybe_unused]]
static std::string to_string(const StructPrimitives& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);

    str += fmt::format("{}rects:\n", indent(1));
    for(size_t i = 0u; i < aObj.rects.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.rects[i])), 2);
    }

    str += fmt::format("{}lines:\n", indent(1));
    for(size_t i = 0u; i < aObj.lines.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.lines[i])), 2);
    }

    str += fmt::format("{}arcs:\n", indent(1));
    for(size_t i = 0u; i < aObj.arcs.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.arcs[i])), 2);
    }

    str += fmt::format("{}ellipses:\n", indent(1));
    for(size_t i = 0u; i < aObj.ellipses.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.ellipses[i])), 2);
    }

    str += fmt::format("{}polygons:\n", indent(1));
    for(size_t i = 0u; i < aObj.polygons.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.polygons[i])), 2);
    }

    str += fmt::format("{}polylines:\n", indent(1));
    for(size_t i = 0u; i < aObj.polylines.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.polylines[i])), 2);
    }

    str += fmt::format("{}commentTexts:\n", indent(1));
    for(size_t i = 0u; i < aObj.commentTexts.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.commentTexts[i])), 2);
    }

    str += fmt::format("{}bitmaps:\n", indent(1));
    for(size_t i = 0u; i < aObj.bitmaps.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.bitmaps[i])), 2);
    }

    str += fmt::format("{}symbolVectors:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolVectors.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.symbolVectors[i])), 2);
    }

    str += fmt::format("{}beziers:\n", indent(1));
    for(size_t i = 0u; i < aObj.beziers.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.beziers[i])), 2);
    }

    return str;
}


inline std::string StructPrimitives::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructPrimitives& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTPRIMITIVES_HPP