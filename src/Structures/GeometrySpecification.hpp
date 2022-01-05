#ifndef GEOMETRYSPECIFICATION_H
#define GEOMETRYSPECIFICATION_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Arc.hpp"
#include "Bezier.hpp"
#include "Bitmap.hpp"
#include "Ellipse.hpp"
#include "SymbolVector.hpp"
#include "Line.hpp"
#include "CommentText.hpp"
#include "Polygon.hpp"
#include "Polyline.hpp"
#include "Rect.hpp"


struct GeometrySpecification
{
    std::string name;

    std::vector<Rect>         rects;
    std::vector<Line>         lines;
    std::vector<Arc>          arcs;
    std::vector<Ellipse>      ellipses;
    std::vector<Polygon>      polygons;
    std::vector<Polyline>     polylines;
    std::vector<CommentText>  commentTexts;
    std::vector<Bitmap>       bitmaps;
    std::vector<SymbolVector> symbolVectors;
    std::vector<Bezier>       beziers;
};


[[maybe_unused]]
static std::string to_string(const GeometrySpecification& geometrySpecification)
{
    std::string str;

    str += "GeometrySpecification:" + newLine();
    str += indent(1) + "name = " + geometrySpecification.name + newLine();

    str += indent(1) + "rects:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.rects.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.rects[i]), 2);
    }

    str += indent(1) + "lines:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.lines.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.lines[i]), 2);
    }

    str += indent(1) + "arcs:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.arcs.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.arcs[i]), 2);
    }

    str += indent(1) + "ellipses:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.ellipses.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.ellipses[i]), 2);
    }

    str += indent(1) + "polygons:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.polygons.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.polygons[i]), 2);
    }

    str += indent(1) + "polylines:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.polylines.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.polylines[i]), 2);
    }

    str += indent(1) + "commentTexts:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.commentTexts.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.commentTexts[i]), 2);
    }

    str += indent(1) + "bitmaps:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.bitmaps.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.bitmaps[i]), 2);
    }

    str += indent(1) + "symbolVectors:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.symbolVectors.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.symbolVectors[i]), 2);
    }

    str += indent(1) + "beziers:" + newLine();
    for(size_t i = 0u; i < geometrySpecification.beziers.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(geometrySpecification.beziers[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const GeometrySpecification& geometrySpecification)
{
    os << to_string(geometrySpecification);

    return os;
}


#endif // GEOMETRYSPECIFICATION_H