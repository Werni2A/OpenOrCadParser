#include <memory>
#include <string>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "Primitives/Point.hpp"
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
#include "Record.hpp"
#include "RecordFactory.hpp"
#include "StreamContext.hpp"
#include "Structures/StructAlias.hpp"
#include "Structures/StructBookMarkSymbolInst.hpp"
#include "Structures/StructBusEntry.hpp"
#include "Structures/StructDevice.hpp"
#include "Structures/StructERCObject.hpp"
#include "Structures/StructERCSymbol.hpp"
#include "Structures/StructGeneralProperties.hpp"
#include "Structures/StructGlobal.hpp"
#include "Structures/StructGlobalSymbol.hpp"
#include "Structures/StructGraphicArcInst.hpp"
#include "Structures/StructGraphicBezierInst.hpp"
#include "Structures/StructGraphicBitMapInst.hpp"
#include "Structures/StructGraphicBoxInst.hpp"
#include "Structures/StructGraphicCommentTextInst.hpp"
#include "Structures/StructGraphicEllipseInst.hpp"
#include "Structures/StructGraphicLineInst.hpp"
#include "Structures/StructGraphicOleEmbedInst.hpp"
#include "Structures/StructGraphicPolygonInst.hpp"
#include "Structures/StructGraphicPolylineInst.hpp"
#include "Structures/StructLibraryPart.hpp"
#include "Structures/StructNetDbIdMapping.hpp"
#include "Structures/StructOffPageConnector.hpp"
#include "Structures/StructOffPageSymbol.hpp"
#include "Structures/StructPackage.hpp"
#include "Structures/StructPartCell.hpp"
#include "Structures/StructPartInst.hpp"
#include "Structures/StructPinShapeSymbol.hpp"
#include "Structures/StructPort.hpp"
#include "Structures/StructPortSymbol.hpp"
#include "Structures/StructSthInHierarchy1.hpp"
#include "Structures/StructSthInHierarchy2.hpp"
#include "Structures/StructSthInHierarchy3.hpp"
#include "Structures/StructSthInPages0.hpp"
#include "Structures/StructSymbolBBox.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructSymbolPinBus.hpp"
#include "Structures/StructSymbolPinScalar.hpp"
#include "Structures/StructT0x10.hpp"
#include "Structures/StructT0x34.hpp"
#include "Structures/StructT0x35.hpp"
#include "Structures/StructT0x45.hpp"
#include "Structures/StructT0x5b.hpp"
#include "Structures/StructTitleBlock.hpp"
#include "Structures/StructTitleBlockSymbol.hpp"
#include "Structures/StructWireBus.hpp"
#include "Structures/StructWireScalar.hpp"


std::unique_ptr<Record> RecordFactory::build(StreamContext& aCtx, Structure aStructure)
{
    switch(aStructure)
    {
        case Structure::Alias:                  return std::make_unique<StructAlias>(aCtx);                  break;
        case Structure::BookMarkSymbolInst:     return std::make_unique<StructBookMarkSymbolInst>(aCtx);     break;
        case Structure::BusEntry:               return std::make_unique<StructBusEntry>(aCtx);               break;
        case Structure::Device:                 return std::make_unique<StructDevice>(aCtx);                 break;
        case Structure::ERCObject:              return std::make_unique<StructERCObject>(aCtx);              break;
        case Structure::ERCSymbol:              return std::make_unique<StructERCSymbol>(aCtx);              break;
        case Structure::Global:                 return std::make_unique<StructGlobal>(aCtx);                 break;
        case Structure::GlobalSymbol:           return std::make_unique<StructGlobalSymbol>(aCtx);           break;
        case Structure::GraphicArcInst:         return std::make_unique<StructGraphicArcInst>(aCtx);         break;
        case Structure::GraphicBezierInst:      return std::make_unique<StructGraphicBezierInst>(aCtx);      break;
        case Structure::GraphicBitMapInst:      return std::make_unique<StructGraphicBitMapInst>(aCtx);      break;
        case Structure::GraphicBoxInst:         return std::make_unique<StructGraphicBoxInst>(aCtx);         break;
        case Structure::GraphicCommentTextInst: return std::make_unique<StructGraphicCommentTextInst>(aCtx); break;
        case Structure::GraphicEllipseInst:     return std::make_unique<StructGraphicEllipseInst>(aCtx);     break;
        case Structure::GraphicLineInst:        return std::make_unique<StructGraphicLineInst>(aCtx);        break;
        case Structure::GraphicOleEmbedInst:    return std::make_unique<StructGraphicOleEmbedInst>(aCtx);    break;
        case Structure::GraphicPolygonInst:     return std::make_unique<StructGraphicPolygonInst>(aCtx);     break;
        case Structure::GraphicPolylineInst:    return std::make_unique<StructGraphicPolylineInst>(aCtx);    break;
        case Structure::LibraryPart:            return std::make_unique<StructLibraryPart>(aCtx);            break;
        case Structure::NetDbIdMapping:         return std::make_unique<StructNetDbIdMapping>(aCtx);         break;
        case Structure::OffPageConnector:       return std::make_unique<StructOffPageConnector>(aCtx);       break;
        case Structure::OffPageSymbol:          return std::make_unique<StructOffPageSymbol>(aCtx);          break;
        case Structure::Package:                return std::make_unique<StructPackage>(aCtx);                break;
        case Structure::PartCell:               return std::make_unique<StructPartCell>(aCtx);               break;
        case Structure::PinShapeSymbol:         return std::make_unique<StructPinShapeSymbol>(aCtx);         break;
        case Structure::PlacedInstance:         return std::make_unique<StructPartInst>(aCtx);               break;
        case Structure::Port:                   return std::make_unique<StructPort>(aCtx);                   break;
        case Structure::PortSymbol:             return std::make_unique<StructPortSymbol>(aCtx);             break;
        case Structure::SthInHierarchy1:        return std::make_unique<StructSthInHierarchy1>(aCtx);        break;
        case Structure::SthInHierarchy2:        return std::make_unique<StructSthInHierarchy2>(aCtx);        break;
        case Structure::SthInHierarchy3:        return std::make_unique<StructSthInHierarchy3>(aCtx);        break;
        case Structure::SthInPages0:            return std::make_unique<StructSthInPages0>(aCtx);            break;
        case Structure::SymbolDisplayProp:      return std::make_unique<StructSymbolDisplayProp>(aCtx);      break;
        case Structure::SymbolPinBus:           return std::make_unique<StructSymbolPinBus>(aCtx);           break;
        case Structure::SymbolPinScalar:        return std::make_unique<StructSymbolPinScalar>(aCtx);        break;
        case Structure::T0x10:                  return std::make_unique<StructT0x10>(aCtx);                  break;
        case Structure::T0x34:                  return std::make_unique<StructT0x34>(aCtx);                  break;
        case Structure::T0x35:                  return std::make_unique<StructT0x35>(aCtx);                  break;
        case Structure::T0x45:                  return std::make_unique<StructT0x45>(aCtx);                  break;
        case Structure::T0x5b:                  return std::make_unique<StructT0x5b>(aCtx);                  break;
        case Structure::TitleBlock:             return std::make_unique<StructTitleBlock>(aCtx);             break;
        case Structure::TitleBlockSymbol:       return std::make_unique<StructTitleBlockSymbol>(aCtx);       break;
        case Structure::WireBus:                return std::make_unique<StructWireBus>(aCtx);                break;
        case Structure::WireScalar:             return std::make_unique<StructWireScalar>(aCtx);             break;
        default:
            {
                const std::string errMsg = fmt::format(
                    "RecordFactory can not yet build {}", to_string(aStructure));
                aCtx.mLogger.error(errMsg);
            }
    }

    return std::unique_ptr<Record>{};
}


std::unique_ptr<PrimBase> RecordFactory::build(StreamContext& aCtx, Primitive aPrimitive)
{
    switch(aPrimitive)
    {
        case Primitive::Rect:         return std::make_unique<PrimRect>(aCtx);         break;
        case Primitive::Line:         return std::make_unique<PrimLine>(aCtx);         break;
        case Primitive::Arc:          return std::make_unique<PrimArc>(aCtx);          break;
        case Primitive::Ellipse:      return std::make_unique<PrimEllipse>(aCtx);      break;
        case Primitive::Polygon:      return std::make_unique<PrimPolygon>(aCtx);      break;
        case Primitive::Polyline:     return std::make_unique<PrimPolyline>(aCtx);     break;
        case Primitive::CommentText:  return std::make_unique<PrimCommentText>(aCtx);  break;
        case Primitive::Bitmap:       return std::make_unique<PrimBitmap>(aCtx);       break;
        case Primitive::SymbolVector: return std::make_unique<PrimSymbolVector>(aCtx); break;
        case Primitive::Bezier:       return std::make_unique<PrimBezier>(aCtx);       break;
        default:
            {
                const std::string errMsg = fmt::format(
                    "RecordFactory can not yet build {}", to_string(aPrimitive));
                aCtx.mLogger.error(errMsg);
            }
    }

    return std::unique_ptr<PrimBase>{};
}
