#ifndef DATAVARIANTS_HPP
#define DATAVARIANTS_HPP


#include <variant>

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
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructGeneralProperties.hpp"
#include "Structures/StructPinIdxMapping.hpp"
#include "Structures/StructPrimitives.hpp"
#include "Structures/StructProperties.hpp"
#include "Structures/StructProperties2.hpp"
#include "Structures/StructSymbolBBox.hpp"
#include "Structures/StructSymbolPinBus.hpp"
#include "Structures/StructSymbolPinScalar.hpp"
#include "Structures/StructT0x1f.hpp"
#include "Structures/StructWireScalar.hpp"


using VariantPrimitive = std::variant<
    PrimArc,
    PrimBezier,
    PrimBitmap,
    PrimCommentText,
    PrimEllipse,
    PrimLine,
    PrimPolygon,
    PrimPolyline,
    PrimRect,
    PrimSymbolVector
>;

using VariantStructure = std::variant<
    StructGeneralProperties,
    StructPinIdxMapping,
    StructPrimitives,
    StructProperties,
    StructProperties2,
    StructSymbolBBox,
    StructSymbolDisplayProp,
    StructSymbolPinBus,
    StructSymbolPinScalar,
    StructT0x1f,
    StructWireScalar
>;


[[maybe_unused]]
inline std::string to_string(const VariantPrimitive& aObj)
{
    std::string str;

    if(std::holds_alternative<PrimArc>(aObj)) { str = to_string(std::get<PrimArc>(aObj)); }
    else if(std::holds_alternative<PrimBezier>(aObj)) { str = to_string(std::get<PrimBezier>(aObj)); }
    else if(std::holds_alternative<PrimBitmap>(aObj)) { str = to_string(std::get<PrimBitmap>(aObj)); }
    else if(std::holds_alternative<PrimCommentText>(aObj)) { str = to_string(std::get<PrimCommentText>(aObj)); }
    else if(std::holds_alternative<PrimEllipse>(aObj)) { str = to_string(std::get<PrimEllipse>(aObj)); }
    else if(std::holds_alternative<PrimLine>(aObj)) { str = to_string(std::get<PrimLine>(aObj)); }
    else if(std::holds_alternative<PrimPolygon>(aObj)) { str = to_string(std::get<PrimPolygon>(aObj)); }
    else if(std::holds_alternative<PrimPolyline>(aObj)) { str = to_string(std::get<PrimPolyline>(aObj)); }
    else if(std::holds_alternative<PrimRect>(aObj)) { str = to_string(std::get<PrimRect>(aObj)); }
    else if(std::holds_alternative<PrimSymbolVector>(aObj)) { str = to_string(std::get<PrimSymbolVector>(aObj)); }
    else
    {
        const std::string msg = fmt::format("{}: Primitive not implemented", __func__);
        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    return str;
}


[[maybe_unused]]
inline std::string to_string(const VariantStructure& aObj)
{
    std::string str;

    if(std::holds_alternative<StructGeneralProperties>(aObj)) { str = to_string(std::get<StructGeneralProperties>(aObj)); }
    else if(std::holds_alternative<StructPinIdxMapping>(aObj)) { str = to_string(std::get<StructPinIdxMapping>(aObj)); }
    else if(std::holds_alternative<StructPrimitives>(aObj)) { str = to_string(std::get<StructPrimitives>(aObj)); }
    else if(std::holds_alternative<StructProperties>(aObj)) { str = to_string(std::get<StructProperties>(aObj)); }
    else if(std::holds_alternative<StructProperties2>(aObj)) { str = to_string(std::get<StructProperties2>(aObj)); }
    else if(std::holds_alternative<StructSymbolBBox>(aObj)) { str = to_string(std::get<StructSymbolBBox>(aObj)); }
    else if(std::holds_alternative<StructSymbolDisplayProp>(aObj)) { str = to_string(std::get<StructSymbolDisplayProp>(aObj)); }
    else if(std::holds_alternative<StructSymbolPinBus>(aObj)) { str = to_string(std::get<StructSymbolPinBus>(aObj)); }
    else if(std::holds_alternative<StructSymbolPinScalar>(aObj)) { str = to_string(std::get<StructSymbolPinScalar>(aObj)); }
    else if(std::holds_alternative<StructT0x1f>(aObj)) { str = to_string(std::get<StructT0x1f>(aObj)); }
    else if(std::holds_alternative<StructWireScalar>(aObj)) { str = to_string(std::get<StructWireScalar>(aObj)); }
    else
    {
        const std::string msg = fmt::format("{}: Structure not implemented", __func__);
        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    return str;
}


#endif // DATAVARIANTS_HPP