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
static std::string to_string(const SymbolVector& symbolVector)
{
    std::string str;

    str += "SymbolVector:" + newLine();
    str += indent(1) + "locX = " + std::to_string(symbolVector.locX) + newLine();
    str += indent(1) + "locY = " + std::to_string(symbolVector.locY) + newLine();
    str += indent(1) + "name = " + symbolVector.name + newLine();

    str += indent(1) + "rects:" + newLine();
    for(size_t i = 0u; i < symbolVector.rects.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(symbolVector.rects[i]), 2);
    }

    str += indent(1) + "lines:" + newLine();
    for(size_t i = 0u; i < symbolVector.lines.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(symbolVector.lines[i]), 2);
    }

    str += indent(1) + "arcs:" + newLine();
    for(size_t i = 0u; i < symbolVector.arcs.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(symbolVector.arcs[i]), 2);
    }

    str += indent(1) + "ellipses:" + newLine();
    for(size_t i = 0u; i < symbolVector.ellipses.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(symbolVector.ellipses[i]), 2);
    }

    str += indent(1) + "polygons:" + newLine();
    for(size_t i = 0u; i < symbolVector.polygons.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(symbolVector.polygons[i]), 2);
    }

    str += indent(1) + "polylines:" + newLine();
    for(size_t i = 0u; i < symbolVector.polylines.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(symbolVector.polylines[i]), 2);
    }

    str += indent(1) + "commentTexts:" + newLine();
    for(size_t i = 0u; i < symbolVector.commentTexts.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(symbolVector.commentTexts[i]), 2);
    }

    str += indent(1) + "beziers:" + newLine();
    for(size_t i = 0u; i < symbolVector.beziers.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(symbolVector.beziers[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const SymbolVector& symbolVector)
{
    os << to_string(symbolVector);

    return os;
}


#endif // SYMBOLVECTOR_H