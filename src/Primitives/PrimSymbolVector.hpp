#ifndef PRIMSYMBOLVECTOR_HPP
#define PRIMSYMBOLVECTOR_HPP


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Primitives/PrimArc.hpp"
#include "Primitives/PrimBezier.hpp"
#include "Primitives/PrimCommentText.hpp"
#include "Primitives/PrimEllipse.hpp"
#include "Primitives/PrimLine.hpp"
#include "Primitives/PrimPolygon.hpp"
#include "Primitives/PrimPolyline.hpp"
#include "Primitives/PrimRect.hpp"


struct PrimSymbolVector
{
    int16_t locX;
    int16_t locY;

    std::string name;

    // @note The positions of the primitives are
    //       relative to the offset provided by
    //       locX and locY.
    std::vector<PrimRect>        rects;
    std::vector<PrimLine>        lines;
    std::vector<PrimArc>         arcs;
    std::vector<PrimEllipse>     ellipses;
    std::vector<PrimPolygon>     polygons;
    std::vector<PrimPolyline>    polylines;
    std::vector<PrimCommentText> commentTexts;
    std::vector<PrimBezier>      beziers;
};


[[maybe_unused]]
static std::string to_string(const PrimSymbolVector& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}locX = {}\n", indent(1), aObj.locX);
    str += fmt::format("{}locY = {}\n", indent(1), aObj.locY);
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

    str += fmt::format("{}beziers:\n", indent(1));
    for(size_t i = 0u; i < aObj.beziers.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.beziers[i])), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PrimSymbolVector& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PRIMSYMBOLVECTOR_HPP