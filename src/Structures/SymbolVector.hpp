#ifndef SYMBOLVECTOR_H
#define SYMBOLVECTOR_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "Arc.hpp"
#include "Bezier.hpp"
#include "CommentText.hpp"
#include "Ellipse.hpp"
#include "Line.hpp"
#include "Polygon.hpp"
#include "Polyline.hpp"
#include "Rect.hpp"


struct SymbolVector
{
    int16_t locX;
    int16_t locY;

    std::string name;

    // @note The positions of the following structures
    //       are relative to the offset provided by
    //       locX and locY.
    std::vector<Rect>        rects;
    std::vector<Line>        lines;
    std::vector<Arc>         arcs;
    std::vector<Ellipse>     ellipses;
    std::vector<Polygon>     polygons;
    std::vector<Polyline>    polylines;
    std::vector<CommentText> commentTexts;
    std::vector<Bezier>      beziers;
};


[[maybe_unused]]
static std::string to_string(const SymbolVector& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "locX = " + std::to_string(aObj.locX) + newLine();
    str += indent(1) + "locY = " + std::to_string(aObj.locY) + newLine();
    str += indent(1) + "name = " + aObj.name + newLine();

    str += indent(1) + "rects:" + newLine();
    for(size_t i = 0u; i < aObj.rects.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.rects[i]), 2);
    }

    str += indent(1) + "lines:" + newLine();
    for(size_t i = 0u; i < aObj.lines.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.lines[i]), 2);
    }

    str += indent(1) + "arcs:" + newLine();
    for(size_t i = 0u; i < aObj.arcs.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.arcs[i]), 2);
    }

    str += indent(1) + "ellipses:" + newLine();
    for(size_t i = 0u; i < aObj.ellipses.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.ellipses[i]), 2);
    }

    str += indent(1) + "polygons:" + newLine();
    for(size_t i = 0u; i < aObj.polygons.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.polygons[i]), 2);
    }

    str += indent(1) + "polylines:" + newLine();
    for(size_t i = 0u; i < aObj.polylines.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.polylines[i]), 2);
    }

    str += indent(1) + "commentTexts:" + newLine();
    for(size_t i = 0u; i < aObj.commentTexts.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.commentTexts[i]), 2);
    }

    str += indent(1) + "beziers:" + newLine();
    for(size_t i = 0u; i < aObj.beziers.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.beziers[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const SymbolVector& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // SYMBOLVECTOR_H