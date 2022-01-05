#ifndef GEOMETRYSTRUCTURE_H
#define GEOMETRYSTRUCTURE_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


// @todo Rename to something like Graphics this would make more sense
enum class GeometryStructure
{
    Rect         = 0x2828,
    Line         = 0x2929,
    Arc          = 0x2a2a,
    Ellipse      = 0x2b2b,
    Polygon      = 0x2c2c,
    Polyline     = 0x2d2d,
    CommentText  = 0x2e2e,
    Bitmap       = 0x2f2f,
    SymbolVector = 0x3030,
    Bezier       = 0x5757
};


[[maybe_unused]]
static GeometryStructure ToGeometryStructure(uint16_t val)
{
    GeometryStructure geometryStructure;

    switch(val)
    {
        case 0x2828: geometryStructure = GeometryStructure::Rect;         break;
        case 0x2929: geometryStructure = GeometryStructure::Line;         break;
        case 0x2a2a: geometryStructure = GeometryStructure::Arc;          break;
        case 0x2b2b: geometryStructure = GeometryStructure::Ellipse;      break;
        case 0x2c2c: geometryStructure = GeometryStructure::Polygon;      break;
        case 0x2d2d: geometryStructure = GeometryStructure::Polyline;     break;
        case 0x2e2e: geometryStructure = GeometryStructure::CommentText;  break;
        case 0x2f2f: geometryStructure = GeometryStructure::Bitmap;       break;
        case 0x3030: geometryStructure = GeometryStructure::SymbolVector; break;
        case 0x5757: geometryStructure = GeometryStructure::Bezier;       break;
        default:
            std::string errorMsg = "GeometryStructure with value 0x" + ToHex(val, 4)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return geometryStructure;
}


[[maybe_unused]]
static std::string to_string(const GeometryStructure& geometryStructure)
{
    std::string str;

    switch(geometryStructure)
    {
        case GeometryStructure::Rect:         str = "Rect";         break;
        case GeometryStructure::Line:         str = "Line";         break;
        case GeometryStructure::Arc:          str = "Arc";          break;
        case GeometryStructure::Ellipse:      str = "Ellipse";      break;
        case GeometryStructure::Polygon:      str = "Polygon";      break;
        case GeometryStructure::Polyline:     str = "Polyline";     break;
        case GeometryStructure::CommentText:  str = "CommentText";  break;
        case GeometryStructure::Bitmap:       str = "Bitmap";       break;
        case GeometryStructure::SymbolVector: str = "SymbolVector"; break;
        case GeometryStructure::Bezier:       str = "Bezier";       break;
        default:
            std::string errorMsg = "GeometryStructure with value 0x" + ToHex(geometryStructure, 4)
                                 + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const GeometryStructure& geometryStructure)
{
    os << to_string(geometryStructure);
    return os;
}


#endif // GEOMETRYSTRUCTURE_H